#pragma once
#include "../../visitor/headers/visitor.h"
#include <unordered_map>

// Evaluation Visitor class
class EvaluationVisitor : public Visitor {
public:
    double result; // To store the evaluation result

    // Constructor to initialize the result
    EvaluationVisitor();

    // Visit methods for different node types
    void visit(Variable* variable) override;
    void visit(Const* constant) override;
    void visit(Input* input) override;
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;
    double evaluate(std::shared_ptr<Node> root, double* inputs);

private:
    double* inputs;
    std::unordered_map<Node*, double> input_map;
    int input_idx = 0;
};
