#include "../../visitor/headers/visitor.h"
#include "../../register-allocator/headers/register_allocator.h"
#include "../../emitter/headers/emitter.h"

#include <unordered_map>

class JITVisitor : public Visitor {
private:
    RegisterAllocator register_allocator;
    RegisterMap register_allocation;

    std::unordered_map<Node*, int> node_displacement;
    std::unordered_map<int, double*> spilled_registers;
    int curr_displacement = 0;
    double* spill_addr;
    
    Emitter emitter;

public:
    JITVisitor(RegisterAllocator register_allocator, Emitter emitter) : register_allocator(register_allocator),  emitter(emitter) {};

    void visit(Variable* node) override;
    void visit(Const* node) override;
    void visit(Input* node) override;
    void visit_operation(OperationNode* node, void (Emitter::*operation)(int, int));
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;

    int get_register(Node* node);
    void spill_register(int reg);
    void reload_register(int reg);
    compiled jit(std::shared_ptr<Node> graph);
};