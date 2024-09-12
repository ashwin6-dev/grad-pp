#include "visitor.h"

#include <unordered_map>
#include <vector>

using RegisterMap = std::unordered_map<Node*, int>;

class RegisterAllocator : public Visitor {
private:
    RegisterMap register_map;
    std::vector<int> available_registers;
    std::vector<int> used_registers;

public:
    void visit(Variable* node) override;
    void visit(Const* node) override;
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;

    RegisterMap allocate_registers(std::shared_ptr<Node> graph);
    void allocate_node(Node* node, int reg = -1);
};