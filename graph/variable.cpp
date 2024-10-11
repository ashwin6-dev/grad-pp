#include "headers/variable.h"
#include "../visitor/headers/visitor.h"

void Variable::accept(Visitor* visitor)
{
    visitor->visit(this);
}

double Variable::get_value()
{
    return value;
}

void Variable::set_value(double value)
{
    this->value = value;
}

double* Variable::get_value_address()
{
    return &value;
}

std::shared_ptr<Variable> make_variable(double value)
{
    return std::make_shared<Variable>(value);
}