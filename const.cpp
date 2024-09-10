#include "headers/const.h"

void Const::accept(Visitor& visitor)
{
    visitor->accept(this);
}

std::shared_ptr<Const> make_variable(double value)
{
    return std::make_shared<Const>(value);
}