#include "headers/const.h"
#include "../visitor/headers/visitor.h"

void Const::accept(Visitor* visitor)
{
    visitor->visit(this);
}

double Const::get_value()
{
    return value;
}

double* Const::get_value_address()
{
    return &value;
}

std::shared_ptr<Const> make_const(double value)
{
    return std::make_shared<Const>(value);
}