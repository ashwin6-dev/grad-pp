#pragma once

#include "../../visitor/headers/visitor.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <unordered_set>

struct LiveInterval {
    Node* node;
    int start;
    int end;
};

using RegisterMap = std::unordered_map<Node*, int>;

class RegisterAllocator : public Visitor {
public:
    RegisterMap allocate_registers(std::shared_ptr<Node> graph);
    
private:
    int next_reg(Node* node);
    void mark_taken(Node* node, int reg);
    void mark_taken_rec(Node* node, int reg);
    void mark_taken_in_sibling_subtree(Node* node, int reg);
    void update_live_interval(Node* node);
    void build_live_intervals(std::shared_ptr<Node> graph);
    void allocate_registers_using_live_analysis();
    std::vector<int> get_available_registers(std::vector<Node*> nodes);
    
    void visit(Variable* node);
    void visit(Const* node);
    void visit(Input* node);
    void visit_operation(OperationNode* node);
    void visit(Add* node);
    void visit(Subtract* node);
    void visit(Multiply* node);
    void visit(Divide* node);
    
    std::unordered_map<Node*, std::vector<int>> taken_registers;
    std::unordered_map<Node*, LiveInterval> live_intervals;
    std::unordered_map<Node*, Node*> right_sibling;
    RegisterMap register_map;
    
    int current_time = 0; // Tracks the current position in the program order
};