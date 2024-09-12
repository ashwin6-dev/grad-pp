#include "headers/const.h"
#include "headers/visitor.h"

void Const::accept(Visitor* visitor)
{
    visitor->visit(this);
}

double Const::get_value()
{
    return value;
}

std::shared_ptr<Const> make_const(double value)
{
    return std::make_shared<Const>(value);
}