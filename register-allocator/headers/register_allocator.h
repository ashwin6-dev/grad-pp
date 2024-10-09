#pragma once

#include "../../visitor/headers/visitor.h"

#include <unordered_map>
#include <vector>

using RegisterMap = std::unordered_map<Node*, int>;

struct LiveInterval {
    Node* node;
    int start;
    int end;
};

class RegisterAllocator : public Visitor {
private:
    RegisterMap register_map;
    std::vector<int> available_registers;
    std::vector<int> used_registers;
    std::unordered_map<Node*, LiveInterval> live_intervals;
    int time;

public:
    void visit(Variable* node) override;
    void visit(Const* node) override;
    void visit(Input* node) override;
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;

    RegisterMap allocate_registers(std::shared_ptr<Node> graph);
    void expire_old_intervals(std::vector<LiveInterval> &intervals, int start_time);
    void update_live_interval(Node* node);
    void allocate_node(Node* node, int reg = -1);
};