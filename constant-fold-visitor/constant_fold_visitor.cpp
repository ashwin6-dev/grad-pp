#include "headers/constant_fold_visitor.h"

std::shared_ptr<Node> ConstantFoldVisitor::constant_fold(std::shared_ptr<Node> graph)
{
    graph->accept(this);
    return result;
}

void ConstantFoldVisitor::visit(Variable* variable) 
{
    result = make_variable(variable->get_value());
}

void ConstantFoldVisitor::visit(Const* constant)
{
    result = make_const(constant->get_value());
}

void ConstantFoldVisitor::visit(Input* input)
{
    result = make_input();
}

void ConstantFoldVisitor::visit(Add* node)
{
    node->get_left()->accept(this);
    std::shared_ptr<Node> left_simplifed = result;

    node->get_right()->accept(this);
    std::shared_ptr<Node> right_simplified = result;

    if (auto const_left = std::dynamic_pointer_cast<Const>(left_simplifed) 
    &&  auto const_right = std::dynamic_pointer_cast<Const>(right_simplified)) {
        result = make_const(const_left->get_value() + const_right->get_value());
        return;
    }

    result = make_add(left_simplifed, right_simplified);
}

void ConstantFoldVisitor::visit(Subtract* node)
{
    node->get_left()->accept(this);
    std::shared_ptr<Node> left_simplifed = result;

    node->get_right()->accept(this);
    std::shared_ptr<Node> right_simplified = result;

    if (auto const_left = std::dynamic_pointer_cast<Const>(left_simplifed) 
    &&  auto const_right = std::dynamic_pointer_cast<Const>(right_simplified)) {
        result = make_const(const_left->get_value() - const_right->get_value());
        return;
    }

    result = make_subtract(left_simplifed, right_simplified);
}

void ConstantFoldVisitor::visit(Multiply* node) 
{
    node->get_left()->accept(this);
    std::shared_ptr<Node> left_simplifed = result;

    node->get_right()->accept(this);
    std::shared_ptr<Node> right_simplified = result;

    if (auto const_left = std::dynamic_pointer_cast<Const>(left_simplifed) 
    &&  auto const_right = std::dynamic_pointer_cast<Const>(right_simplified)) {
        result = make_const(const_left->get_value() * const_right->get_value());
        return;
    }

    result = make_multiply(left_simplifed, right_simplified);
}

void ConstantFoldVisitor::visit(Divide* node) 
{
    node->get_left()->accept(this);
    std::shared_ptr<Node> left_simplifed = result;

    node->get_right()->accept(this);
    std::shared_ptr<Node> right_simplified = result;

    if (auto const_left = std::dynamic_pointer_cast<Const>(left_simplifed) 
    &&  auto const_right = std::dynamic_pointer_cast<Const>(right_simplified)) {
        result = make_const(const_left->get_value() / const_right->get_value());
        return;
    }

    result = make_divide(left_simplifed, right_simplified);
}