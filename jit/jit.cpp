#include "headers/jit.h"
#include <iostream>

compiled JITVisitor::jit(std::shared_ptr<Node> graph)
{
    curr_displacement = 0;
    node_displacement.clear();
    emitter.clear();
    register_allocation = register_allocator.allocate_registers(graph);
    graph->accept(this);

    emitter.movesd_reg_reg(register_allocation[graph.get()], 0);
    compiled func = emitter.compile();

    return func;
}

void JITVisitor::visit(Variable* node) 
{
    emitter.movsd_pointer_xmm(node->get_value_address(), register_allocation[node]);
}

void JITVisitor::visit(Const* node)
{
    emitter.movsd_pointer_xmm(node->get_value_address(), register_allocation[node]);
}

void JITVisitor::visit(Input* node)
{
    if (node_displacement.count(node) == 0) {
        node_displacement[node] = curr_displacement;
        curr_displacement += 8;
    }

    int displacement = node_displacement[node];
    emitter.movesd_memory_reg(register_allocation[node], displacement);
}

void JITVisitor::visit(Add* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();
    
    int right_register = register_allocation[right.get()];
    int left_register = register_allocation[left.get()];
    int node_register = register_allocation[node];

    left->accept(this);
    emitter.push_xmm(left_register);
    right->accept(this);
    emitter.pop_xmm(left_register);

    emitter.addsd(right_register, left_register);
    emitter.movesd_reg_reg(left_register, node_register);
}

void JITVisitor::visit(Subtract* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    int right_register = register_allocation[right.get()];
    int left_register = register_allocation[left.get()];
    int node_register = register_allocation[node];

    left->accept(this);
    emitter.push_xmm(left_register);
    right->accept(this);
    emitter.pop_xmm(left_register);

    emitter.subsd(right_register, left_register);
    emitter.movesd_reg_reg(left_register, node_register);
}

void JITVisitor::visit(Multiply* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    int right_register = register_allocation[right.get()];
    int left_register = register_allocation[left.get()];
    int node_register = register_allocation[node];

    left->accept(this);
    emitter.push_xmm(left_register);
    right->accept(this);
    emitter.pop_xmm(left_register);
    
    emitter.mulsd(right_register, left_register);
    emitter.movesd_reg_reg(left_register, node_register);
}

void JITVisitor::visit(Divide* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    int right_register = register_allocation[right.get()];
    int left_register = register_allocation[left.get()];
    int node_register = register_allocation[node];

    left->accept(this);
    emitter.push_xmm(left_register);
    right->accept(this);
    emitter.pop_xmm(left_register);

    
    emitter.divsd(right_register, left_register);
    emitter.movesd_reg_reg(left_register, node_register);
}