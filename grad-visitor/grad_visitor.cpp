#include "headers/grad_visitor.h"

GradVisitor make_grad_visitor()
{
    GradVisitor gv;
    return gv;
}

GradientMap GradVisitor::backward(std::shared_ptr<Node> graph)
{
    gradient_map[graph.get()] = make_const(1.0);
    graph->accept(this);

    return var_gradients;
}

void GradVisitor::apply_gradient(Node* node, std::shared_ptr<Node> gradient) {
    if (auto variable_node = dynamic_cast<Variable*>(node)) {
        if (!gradient_map.count(variable_node)) {
            gradient_map[variable_node] = gradient;
        } else {
            gradient_map[variable_node] = make_add(gradient_map[variable_node], gradient);
        }
        return;
    }

    gradient_map[node] = gradient;
}

void GradVisitor::visit(Variable* node)
{
    var_gradients[node] = gradient_map[node];
}

void GradVisitor::visit(Add* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    apply_gradient(left.get(), gradient_map[node]);
    apply_gradient(right.get(), gradient_map[node]);

    left->accept(this);
    right->accept(this);
}

void GradVisitor::visit(Subtract* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    apply_gradient(left.get(), gradient_map[node]);

    std::shared_ptr<Const> neg = make_const(-1.0);
    std::shared_ptr<Node> new_gradient = make_multiply(neg, gradient_map[node]);

    apply_gradient(right.get(), new_gradient);

    left->accept(this);
    right->accept(this);
}

void GradVisitor::visit(Multiply* node)
{
    std::shared_ptr<Node> left = node->get_left();
    std::shared_ptr<Node> right = node->get_right();

    apply_gradient(left.get(), make_multiply(right, gradient_map[node]));
    apply_gradient(right.get(), make_multiply(left, gradient_map[node]));

    left->accept(this);
    right->accept(this);
}

void GradVisitor::visit(Divide* node)
{
    std::shared_ptr<Node> left = node->get_left();   // u (numerator)
    std::shared_ptr<Node> right = node->get_right(); // v (denominator)

    // Gradient for the numerator (u): d(u/v)/du = 1/v
    apply_gradient(left.get(), make_divide(gradient_map[node], right));

    // Gradient for the denominator (v): d(u/v)/dv = -u / (v^2)
    std::shared_ptr<Const> neg = make_const(-1.0);
    std::shared_ptr<Node> numerator = make_multiply(neg, left);
    std::shared_ptr<Node> denom_squared = make_multiply(right, right);
    std::shared_ptr<Node> gradient_right = make_divide(numerator, denom_squared);

    apply_gradient(right.get(), make_multiply(gradient_map[node], gradient_right));

    left->accept(this);
    right->accept(this);
}

void GradVisitor::visit(Const* node) {}
void GradVisitor::visit(Input* node) {}