#include "../../visitor/headers/visitor.h"
#include "../../register-allocator/headers/register_allocator.h"
#include "../../emitter/headers/emitter.h"

#include <unordered_map>

class JITVisitor : public Visitor {
private:
    RegisterAllocator register_allocator;
    RegisterMap register_allocation;
    
    Emitter emitter;

public:
    JITVisitor(RegisterAllocator register_allocator, Emitter emitter) : register_allocator(register_allocator),  emitter(emitter) {};

    void visit(Variable* node) override;
    void visit(Const* node) override;
    void visit(Input* node) override;
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;

    void jit(std::shared_ptr<Node> graph);
};