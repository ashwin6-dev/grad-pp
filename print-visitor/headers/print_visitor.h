#pragma once

#include "../../visitor/headers/visitor.h"
#include <iostream>

class PrintVisitor : public Visitor {
public:

    void visit(Variable* variable) override;
    void visit(Const* constant) override;
    void visit(Input* input) override;
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;
};