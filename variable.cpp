#include "headers/variable.h"

void Variable::accept(Visitor& visitor)
{
    visitor->accept(this);
}

std::shared_ptr<Variable> make_variable(double value)
{
    return std::make_shared<Variable>(value);
}