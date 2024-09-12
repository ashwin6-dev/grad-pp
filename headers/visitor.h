#pragma once

#include "variable.h"
#include "const.h"
#include "ops.h"

class Visitor {
public:
    virtual void visit(Variable* node) = 0;
    virtual void visit(Const* node) = 0;
    virtual void visit(Add* node) = 0;
    virtual void visit(Subtract* node) = 0;
    virtual void visit(Multiply* node) = 0;
    virtual void visit(Divide* node) = 0;
};