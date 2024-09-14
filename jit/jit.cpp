#include "headers/jit.h"

void JITVisitor::jit(std::shared_ptr<Node> graph)
{
    register_allocation = register_allocator.allocate_registers(graph);
}

void JITVisitor::visit(Variable* node) {}
void JITVisitor::visit(Const* node) {}

void JITVisitor::visit(Add* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    left->accept(this);
    right->accept(this);

    emitter.movesd_reg_reg(register_allocation[left.get()], register_allocation[node]);
    emitter.addsd(register_allocation[right.get()], register_allocation[node]);
}

void JITVisitor::visit(Sub* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    left->accept(this);
    right->accept(this);
    
    emitter.movesd_reg_reg(register_allocation[left.get()], register_allocation[node]);
    emitter.subsd(register_allocation[right.get()], register_allocation[node]);
}

void JITVisitor::visit(Mul* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    left->accept(this);
    right->accept(this);

    emitter.movesd_reg_reg(register_allocation[left.get()], register_allocation[node]);
    emitter.mulsd(register_allocation[right.get()], register_allocation[node]);
}

void JITVisitor::visit(Div* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    left->accept(this);
    right->accept(this);

    emitter.movesd_reg_reg(register_allocation[left.get()], register_allocation[node]);
    emitter.divsd(register_allocation[right.get()], register_allocation[node]);
}