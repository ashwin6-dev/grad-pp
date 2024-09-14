#pragma once

#include "../../graph/headers/variable.h"
#include "../../graph/headers/const.h"
#include "../../graph/headers/ops.h"
#include "../../graph/headers/input.h"

class Visitor {
public:
    virtual void visit(Variable* node) = 0;
    virtual void visit(Const* node) = 0;
    virtual void visit(Input* node) = 0;
    virtual void visit(Add* node) = 0;
    virtual void visit(Subtract* node) = 0;
    virtual void visit(Multiply* node) = 0;
    virtual void visit(Divide* node) = 0;
};