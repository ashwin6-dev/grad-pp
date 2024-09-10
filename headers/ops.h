#include "node.h"
#include <memory>

class OperationNode : public Node {
public:
    OperationNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : left(left), right(right) {}

protected:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

class Add : public OperationNode {
public:
    Add(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : OperationNode(left, right) {}

    void accept(Visitor& visit) override;
};

class Subtract : public OperationNode {
public:
    Subtract(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : OperationNode(left, right) {}

    void accept(Visitor& visit) override;
};

class Multiply : public OperationNode {
public:
    Multiply(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : OperationNode(left, right) {}

    void accept(Visitor& visit) override;
};

class Divide : public OperationNode {
public:
    Divide(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : OperationNode(left, right) {}

    void accept(Visitor& visit) override;
};
