#include "headers/input.h"
#include "../visitor/headers/visitor.h"

void Input::accept(Visitor* visitor)
{
    visitor->visit(this);
}

std::shared_ptr<Input> make_input()
{
    return std::make_shared<Input>();
}