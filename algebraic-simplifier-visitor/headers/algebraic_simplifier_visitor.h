#pragma once

#include "../../visitor/headers/visitor.h"
#include <iostream>
#include <unordered_map>

class AlgebraicSimplifier : public Visitor {
public:
    void visit(Variable* variable) override;
    void visit(Const* constant) override;
    void visit(Input* input) override;
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;

    std::shared_ptr<Node> simplify(std::shared_ptr<Node> graph);
private:
    std::shared_ptr<Node> result;
    std::unordered_map<std::shared_ptr<Node>, Node*> variable_map;
};