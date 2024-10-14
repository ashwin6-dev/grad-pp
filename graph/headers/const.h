#pragma once

#include "node.h"

class Const : public Node {
public:
    Const(double value) : value(value) {};
    void accept(Visitor* visit) override;
    double get_value();
    double* get_value_address();
    
private:
    double value;
};

std::shared_ptr<Const> make_const(double value);