#pragma once

#include "../../visitor/headers/visitor.h"
#include <iostream>

class Variable;
class Const;
class Add;
class Subtract;
class Multiply;
class Divide;

class PrintVisitor : public Visitor {
public:

    void visit(Variable* variable) override;
    void visit(Const* constant) override;
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;
};