#pragma once

#include "visitor.h"
#include "variable.h"
#include "const.h"
#include "ops.h"
#include <iostream>

// Forward declarations of node types
class Variable;
class Const;
class Add;
class Subtract;
class Multiply;
class Divide;

// PrintVisitor class to print the tree structure
class PrintVisitor : public Visitor {
public:
    // Visit method for Variable node
    void visit(Variable* variable) override;

    // Visit method for Constant node
    void visit(Const* constant) override;

    // Visit method for Add node
    void visit(Add* node) override;

    // Visit method for Subtract node
    void visit(Subtract* node) override;

    // Visit method for Multiply node
    void visit(Multiply* node) override;

    // Visit method for Divide node
    void visit(Divide* node) override;
};