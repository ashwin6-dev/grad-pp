#include "headers/register_allocator.h"
#include <algorithm>

RegisterMap RegisterAllocator::allocate_registers(std::shared_ptr<Node> graph)
{
    available_registers = std::vector<int> {};

    int MAX_REGISTERS = 16;

    for (int reg = MAX_REGISTERS; reg >= 0; reg--) {
        available_registers.push_back(reg);
    }

    allocate_node(graph.get());

    return register_map;
}

void RegisterAllocator::allocate_node(Node* node, int reg)
{
    if (register_map.count(node) || available_registers.empty()) return;

    if (reg != -1) {
        register_map[node] = reg;
        node->accept(this);
        return;
    }

    int next_reg = available_registers.back();
    available_registers.pop_back();
    used_registers.push_back(next_reg);
    register_map[node] = next_reg;

    node->accept(this);
}

void RegisterAllocator::visit(Variable* node)
{
    allocate_node(node);
}

void RegisterAllocator::visit(Const* node)
{
    allocate_node(node);
}

void RegisterAllocator::visit(Input* node)
{
    allocate_node(node);
}

void RegisterAllocator::visit(Add* node)
{
    allocate_node(node);

    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    allocate_node(left.get(), register_map[node]);
    allocate_node(right.get());
}

void RegisterAllocator::visit(Subtract* node)
{
    allocate_node(node);

    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    allocate_node(left.get(), register_map[node]);
    allocate_node(right.get());
}

void RegisterAllocator::visit(Multiply* node)
{
    allocate_node(node);

    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    allocate_node(left.get(), register_map[node]);
    allocate_node(right.get());
}

void RegisterAllocator::visit(Divide* node)
{
    allocate_node(node);

    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    allocate_node(left.get(), register_map[node]);
    allocate_node(right.get());
}