#include "headers/register_allocator.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

RegisterMap RegisterAllocator::allocate_registers(std::shared_ptr<Node> graph) {
    build_live_intervals(graph);
    allocate_registers_using_live_analysis();
    return register_map;
}

void RegisterAllocator::build_live_intervals(std::shared_ptr<Node> graph) {
    current_time = 0;
    register_map.clear();
    
    graph->accept(this);
}

void RegisterAllocator::mark_taken(Node* node, int reg) {
    if (taken_registers.count(node) == 0) {
        taken_registers[node] = {};
    }
    taken_registers[node].push_back(reg);
}

void RegisterAllocator::mark_taken_rec(Node* node, int reg) {
    mark_taken(node, reg);
    if (auto* op_node = dynamic_cast<OperationNode*>(node)) {
        mark_taken_rec(op_node->get_left().get(), reg);
        mark_taken_rec(op_node->get_right().get(), reg);
    }
}

void RegisterAllocator::mark_taken_in_sibling_subtree(Node* node, int reg) {
    if (right_sibling.count(node) > 0) {
        mark_taken_rec(right_sibling[node], reg);
    }
}

void RegisterAllocator::update_live_interval(Node* node) {
    if (live_intervals.count(node)) {
        LiveInterval &lv = live_intervals[node];
        lv.start = std::min(lv.start, current_time);
        lv.end = std::max(lv.end, current_time);
    } else {
        live_intervals[node] = {node, current_time, current_time};
    }
}

void RegisterAllocator::visit(Variable* node) {
    update_live_interval(node);
    current_time++;
}

void RegisterAllocator::visit(Const* node) {
    update_live_interval(node);
    current_time++;
}

void RegisterAllocator::visit(Input* node) {
    update_live_interval(node);
    current_time++;
}

void RegisterAllocator::visit_operation(OperationNode* node) {
    int saved_time = current_time;
    Node* left_ptr = node->get_left().get();
    current_time = saved_time;
    Node* right_ptr = node->get_right().get();

    left_ptr->accept(this);
    right_ptr->accept(this);

    right_sibling[left_ptr] = right_ptr; 

    update_live_interval(node);
    current_time++;
}

std::vector<int> RegisterAllocator::get_available_registers(std::vector<Node*> nodes)
{
    std::vector<int> taken;

    for (Node* node : nodes) {
        if (register_map.count(node)) {
            taken.push_back(register_map[node]);
        }
    }
    
    std::vector<int> available_registers;
    for (int i = 0; i <= 15; i++) {
        if (std::find(taken.begin(), taken.end(), i) == taken.end())
            available_registers.push_back(i);
    }

    return available_registers;
}

void RegisterAllocator::allocate_registers_using_live_analysis() {
    std::unordered_map<int, std::vector<Node*>> active_intervals;
    int max_time = 0;

    // Going interval-by-interval and seeing which nodes are active in each interval
    for (const auto& interval : live_intervals) {
        Node* node = interval.first;
        int start = interval.second.start;
        int end = interval.second.end;

        max_time = std::max(max_time, end);

        for (int t = start; t <= end; t++) {
            if (active_intervals.count(t) == 0) {
                active_intervals[t] = std::vector<Node*> {};
            }

            active_intervals[t].push_back(node);
        }
    }

    // Allocates registers going through each interval
    for (int t = 0; t <= max_time; t++) {
        std::vector<Node*> nodes = active_intervals[t];

        std::vector<int> available_registers = get_available_registers(nodes);
        for (Node* node : nodes) {
            if (register_map.count(node) == 0) {
                int reg = -1;

                // Calculates the next available register for this node
                std::vector<int> add_back;
                while (
                    (reg == -1 || std::find(taken_registers[node].begin(), taken_registers[node].end(), reg) != taken_registers[node].end())
                    && available_registers.size()
                ) {
                    if (reg > -1) add_back.push_back(reg);
                    reg = available_registers.back();
                    available_registers.pop_back();
                }

                for (int reg : add_back) {
                    available_registers.push_back(reg);
                }

                if (reg == -1) {
                    std::cout << "no available registers" << std::endl;
                    continue;
                }
                
                register_map[node] = reg;
                mark_taken_in_sibling_subtree(node, reg);
            }
        }
    }
}

void RegisterAllocator::visit(Add* node) {
    visit_operation(dynamic_cast<OperationNode*>(node));
}

void RegisterAllocator::visit(Subtract* node) {
    visit_operation(dynamic_cast<OperationNode*>(node));
}

void RegisterAllocator::visit(Multiply* node) {
    visit_operation(dynamic_cast<OperationNode*>(node));
}

void RegisterAllocator::visit(Divide* node) {
    visit_operation(dynamic_cast<OperationNode*>(node));
}