#pragma once

#include "node.h"

class Variable : public Node {
public:
    Variable(double value) : value(value) {};
    void accept(Visitor& visit) override;
};