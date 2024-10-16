#include "../../visitor/headers/visitor.h"

#include <unordered_map>

using GradientMap = std::unordered_map<Node*, std::shared_ptr<Node>>;

class GradVisitor : public Visitor {
private:
    GradientMap gradient_map;
    GradientMap var_gradients;

public:
    void visit(Variable* node) override;
    void visit(Const* node) override;
    void visit(Input* node) override;
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;

    GradientMap backward(std::shared_ptr<Node> graph);
    void apply_gradient(Node* node, std::shared_ptr<Node> gradient);
};

GradVisitor make_grad_visitor();