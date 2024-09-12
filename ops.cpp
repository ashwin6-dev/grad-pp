#include "headers/ops.h"
#include "headers/visitor.h"

std::shared_ptr<Node> OperationNode::get_left()
{
    return left;
}

std::shared_ptr<Node> OperationNode::get_right()
{
    return right;
}

void Add::accept(Visitor* visitor)
{
    visitor->visit(this);
}

void Subtract::accept(Visitor* visitor)
{
    visitor->visit(this);
}

void Multiply::accept(Visitor* visitor)
{
    visitor->visit(this);
}

void Divide::accept(Visitor* visitor)
{
    visitor->visit(this);
}

std::shared_ptr<Add> make_add(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
{
    return std::make_shared<Add>(left, right);
}

std::shared_ptr<Subtract> make_subtract(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
{
    return std::make_shared<Subtract>(left, right);
}

std::shared_ptr<Multiply> make_multiply(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
{
    return std::make_shared<Multiply>(left, right);
}

std::shared_ptr<Divide> make_divide(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
{
    return std::make_shared<Divide>(left, right);
}