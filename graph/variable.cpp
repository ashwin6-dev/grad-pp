#include "headers/variable.h"
#include "../visitors/headers/visitor.h"

void Variable::accept(Visitor* visitor)
{
    visitor->visit(this);
}

double Variable::get_value()
{
    return value;
}

std::shared_ptr<Variable> make_variable(double value)
{
    return std::make_shared<Variable>(value);
}