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

void JITVisitor::reload_register(int reg)
{
    emitter.movsd_pointer_xmm(spilled_registers[reg], reg);
    spilled_registers.erase(reg);
}

void JITVisitor::spill_register(int reg)
{
    emitter.movsd_xmm_pointer(reg, spill_addr);
    spilled_registers[reg] = spill_addr;
    spill_addr++;
}

int JITVisitor::get_register(Node* node)
{
    if (register_allocation.count(node)) {
        int reg = register_allocation[node];
        return reg;
    }

    int reg = 0;
    for (int i = 0; i < 16; i++) {
        if (!spilled_registers.count(i)) {
            reg = i;
            break;
        }
    }

    spill_register(reg);
    return reg;
}

void JITVisitor::visit(Variable* node) 
{
    emitter.movsd_pointer_xmm(node->get_value_address(), get_register(node));
}

void JITVisitor::visit(Const* node)
{
    emitter.movsd_pointer_xmm(node->get_value_address(), get_register(node));
}

void JITVisitor::visit(Input* node)
{
    if (node_displacement.count(node) == 0) {
        node_displacement[node] = curr_displacement;
        curr_displacement += 8;
    }

    int displacement = node_displacement[node];
    emitter.movesd_memory_reg(get_register(node), displacement);
}

void JITVisitor::visit_operation(OperationNode* node, void (Emitter::*operation)(int, int))
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    int right_register = get_register(right.get());
    int left_register = get_register(left.get());
    int node_register = get_register(node);

    left->accept(this);
    emitter.push_xmm(left_register);
    right->accept(this);
    emitter.pop_xmm(left_register);

    (emitter.*operation)(right_register, left_register);
    emitter.movesd_reg_reg(left_register, node_register);
}

void JITVisitor::visit(Add* node)
{
    visit_operation(node, &Emitter::addsd);
}

void JITVisitor::visit(Subtract* node)
{
    visit_operation(node, &Emitter::subsd);
}

void JITVisitor::visit(Multiply* node)
{
    visit_operation(node, &Emitter::mulsd);
}

void JITVisitor::visit(Divide* node)
{
    visit_operation(node, &Emitter::divsd);
}