#include "headers/jit.h"

compiled JITVisitor::jit(std::shared_ptr<Node> graph)
{
    curr_displacement = 0;
    register_allocation = register_allocator.allocate_registers(graph);
    graph->accept(this);

    return emitter.compile();
}

void JITVisitor::visit(Variable* node) 
{
    emitter.movsd_imm_to_xmm(node->get_value(), register_allocation[node]);
}

void JITVisitor::visit(Const* node)
{
    emitter.movsd_imm_to_xmm(node->get_value(), register_allocation[node]);
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

    left->accept(this);
    right->accept(this);

    emitter.movesd_reg_reg(register_allocation[left.get()], register_allocation[node]);
    emitter.addsd(register_allocation[right.get()], register_allocation[node]);
}

void JITVisitor::visit(Subtract* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    left->accept(this);
    right->accept(this);
    
    emitter.movesd_reg_reg(register_allocation[left.get()], register_allocation[node]);
    emitter.subsd(register_allocation[right.get()], register_allocation[node]);
}

void JITVisitor::visit(Multiply* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    left->accept(this);
    right->accept(this);

    emitter.movesd_reg_reg(register_allocation[left.get()], register_allocation[node]);
    emitter.mulsd(register_allocation[right.get()], register_allocation[node]);
}

void JITVisitor::visit(Divide* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    left->accept(this);
    right->accept(this);

    emitter.movesd_reg_reg(register_allocation[left.get()], register_allocation[node]);
    emitter.divsd(register_allocation[right.get()], register_allocation[node]);
}