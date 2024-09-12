#pragma once

#include "node.h"

class Variable : public Node {
public:
    Variable(double value) : value(value) {};
    void accept(Visitor* visit) override;
    double get_value();
private:
    double value;
};

std::shared_ptr<Variable> make_variable(double value);