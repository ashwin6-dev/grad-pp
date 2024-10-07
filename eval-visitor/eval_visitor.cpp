#include "headers/eval_visitor.h"

// Constructor to initialize the result
EvaluationVisitor::EvaluationVisitor() : result(0.0) {}

// Visit for Variable node
void EvaluationVisitor::visit(Variable* variable) {
    result = variable->get_value();
}

// Visit for Constant node
void EvaluationVisitor::visit(Const* constant) {
    result = constant->get_value();
}

// Visit for Input node
void EvaluationVisitor::visit(Input* input) {
    // Assuming Input node is implemented to return a value
    if (input_map.count(input) == 0) {
        input_map[input] = inputs[input_idx];
        input_idx++;
    }

    result = input_map[input];
}

// Visit for Add node
void EvaluationVisitor::visit(Add* node) {
    node->get_left()->accept(this); // Evaluate left operand
    double left_value = result; // Store left value
    node->get_right()->accept(this); // Evaluate right operand
    result = left_value + result; // Calculate the sum
}

// Visit for Subtract node
void EvaluationVisitor::visit(Subtract* node) {
    node->get_left()->accept(this); // Evaluate left operand
    double left_value = result; // Store left value
    node->get_right()->accept(this); // Evaluate right operand
    result = left_value - result; // Calculate the difference
}

// Visit for Multiply node
void EvaluationVisitor::visit(Multiply* node) {
    node->get_left()->accept(this); // Evaluate left operand
    double left_value = result; // Store left value
    node->get_right()->accept(this); // Evaluate right operand
    result = left_value * result; // Calculate the product
}

// Visit for Divide node
void EvaluationVisitor::visit(Divide* node) {
    node->get_left()->accept(this); // Evaluate left operand
    double left_value = result; // Store left value
    node->get_right()->accept(this); // Evaluate right operand
    double right_value = result; // Store right value

    result = left_value / right_value; // Calculate the quotient
}

// Method to evaluate the graph with given inputs
double EvaluationVisitor::evaluate(std::shared_ptr<Node> root, double* inputs) {
    result = 0;
    this->inputs = inputs;
    root->accept(this);

    return result;
}
