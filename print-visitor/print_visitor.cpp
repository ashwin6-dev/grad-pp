#include "headers/print_visitor.h"

// Visit for Variable node
void PrintVisitor::visit(Variable* variable) {
    std::cout << "Variable[" << variable->get_value() << "]";
}

// Visit for Constant node
void PrintVisitor::visit(Const* constant) {
    std::cout << "Const[" << constant->get_value() << "]";
}

// Visit for Add node
void PrintVisitor::visit(Add* node) {
    std::cout << "(";
    node->get_left()->accept(this);
    std::cout << " + ";
    node->get_right()->accept(this);
    std::cout << ")";
}

// Visit for Subtract node
void PrintVisitor::visit(Subtract* node) {
    std::cout << "(";
    node->get_left()->accept(this);
    std::cout << " - ";
    node->get_right()->accept(this);
    std::cout << ")";
}

// Visit for Multiply node
void PrintVisitor::visit(Multiply* node) {
    std::cout << "(";
    node->get_left()->accept(this);
    std::cout << " * ";
    node->get_right()->accept(this);
    std::cout << ")";
}

// Visit for Divide node
void PrintVisitor::visit(Divide* node) {
    std::cout << "(";
    node->get_left()->accept(this);
    std::cout << " / ";
    node->get_right()->accept(this);
    std::cout << ")";
}

