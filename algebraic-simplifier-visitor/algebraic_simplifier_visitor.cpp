#include "headers/constant_fold_visitor.h"

std::shared_ptr<Node> AlgebraicSimplifier::simplify(std::shared_ptr<Node> graph)
{
    graph->accept(this);
    return result;
}

void AlgebraicSimplifier::visit(Variable* variable) 
{
    result = make_variable(variable->get_value());
    variable_map[result] = variable;
}

void AlgebraicSimplifier::visit(Const* constant)
{
    result = make_const(constant->get_value());
}

void AlgebraicSimplifier::visit(Input* input)
{
    result = make_input();
}

void AlgebraicSimplifier::visit(Add* node)
{
    node->get_left()->accept(this);
    std::shared_ptr<Node> left_simplifed = result;

    node->get_right()->accept(this);
    std::shared_ptr<Node> right_simplified = result;

    if (variable_map.count(left_simplifed) && variable_map.count(right_simplified)) {
        if (variable_map[left_simplifed] == variable_map[right_simplified]) {
            result = make_multiply(make_const(2.0), left_simplifed);
            return;
        }
    }

    result = make_add(left_simplifed, right_simplified)
}

void AlgebraicSimplifier::visit(Subtract* node)
{
    node->get_left()->accept(this);
    std::shared_ptr<Node> left_simplifed = result;

    node->get_right()->accept(this);
    std::shared_ptr<Node> right_simplified = result;

    if (variable_map.count(left_simplifed) && variable_map.count(right_simplified)) {
        if (variable_map[left_simplifed] == variable_map[right_simplified]) {
            result = make_const(0.0);
            return;
        }
    }

    result = make_subtract(left_simplifed, right_simplified)
}

void AlgebraicSimplifier::visit(Multiply* node) 
{
    node->get_left()->accept(this);
    std::shared_ptr<Node> left_simplifed = result;

    node->get_right()->accept(this);
    std::shared_ptr<Node> right_simplified = result;

    result = make_multiply(left_simplifed, right_simplified)
}

void AlgebraicSimplifier::visit(Divide* node) 
{
    node->get_left()->accept(this);
    std::shared_ptr<Node> left_simplifed = result;

    node->get_right()->accept(this);
    std::shared_ptr<Node> right_simplified = result;

    if (variable_map.count(left_simplifed) && variable_map.count(right_simplified)) {
        if (variable_map[left_simplifed] == variable_map[right_simplified]) {
            result = make_const(1.0);
            return;
        }
    }

    result = make_divide(left_simplifed, right_simplified)
}