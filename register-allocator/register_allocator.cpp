#include "headers/register_allocator.h"
#include <algorithm>
#include <iostream>

RegisterMap RegisterAllocator::allocate_registers(std::shared_ptr<Node> graph)
{
    available_registers.clear();
    live_intervals.clear();
    register_map.clear();
    time = 0;
    graph->accept(this);

    std::vector<LiveInterval> sorted_intervals;
    for (const auto& entry : live_intervals) {
        sorted_intervals.push_back(entry.second);
    }

    std::sort(sorted_intervals.begin(), sorted_intervals.end(), [](const LiveInterval& a, const LiveInterval& b) {
        return a.start < b.start;
    });

    int MAX_REGISTERS = 15;

    for (int reg = 0; reg <= MAX_REGISTERS; reg++) {
        available_registers.push_back(reg);
    }

    for (size_t i = 0; i < sorted_intervals.size();) {
        Node* node = sorted_intervals[i].node;

        // std::cout << sorted_intervals[i].start << " " << sorted_intervals[i].end << std::endl;

        int prev_size = sorted_intervals.size();
        expire_old_intervals(sorted_intervals, sorted_intervals[i].start);
        int new_size = sorted_intervals.size();

        if (!available_registers.empty()) {
            int reg = available_registers.back();
            available_registers.pop_back();
            register_map[node] = reg;
            // std::cout << reg << std::endl;
        } else {
            std::cout << "no registers available" << std::endl;
        }

        i = i - (prev_size - new_size) + 1;
    }

    return register_map;
}

void RegisterAllocator::expire_old_intervals(std::vector<LiveInterval>& intervals, int start_time)
{
    for (auto it = intervals.begin(); it != intervals.end(); ) {
        if (it->end < start_time) {
            available_registers.push_back(register_map[it->node]);
            it = intervals.erase(it);
        } else {
            ++it;
        }
    }
}

void RegisterAllocator::allocate_node(Node* node, int reg)
{
}

void RegisterAllocator::visit(Variable* node) {
    update_live_interval(node);
}

void RegisterAllocator::visit(Const* node) {
    update_live_interval(node);
}

void RegisterAllocator::visit(Input* node) {
    update_live_interval(node);
}

void RegisterAllocator::visit(Add* node) {
    update_live_interval(node);
    time++;

    int saved_time = time;
    node->get_left()->accept(this);

    time = saved_time;
    node->get_right()->accept(this);

    update_live_interval(node->get_left().get());
}

void RegisterAllocator::visit(Subtract* node) {
    update_live_interval(node);
    time++;

    int saved_time = time;
    node->get_left()->accept(this);

    time = saved_time;
    node->get_right()->accept(this);

    update_live_interval(node->get_left().get());
}

void RegisterAllocator::visit(Multiply* node) {
    update_live_interval(node);
    time++;

    int saved_time = time;
    node->get_left()->accept(this);

    time = saved_time;
    node->get_right()->accept(this);

    update_live_interval(node->get_left().get());
}

void RegisterAllocator::visit(Divide* node) {
    update_live_interval(node);
    time++;

    int saved_time = time;
    node->get_left()->accept(this);

    time = saved_time;
    node->get_right()->accept(this);

    update_live_interval(node->get_left().get());
}

void RegisterAllocator::update_live_interval(Node* node) {
    if (live_intervals.count(node)) {
        LiveInterval& lv = live_intervals[node];
        lv.start = std::min(lv.start, time);
        lv.end = std::max(lv.end, time);
    } else {
        LiveInterval lv = { node, time, time };
        live_intervals[node] = lv;
    }
}