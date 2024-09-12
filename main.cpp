#include "./visitors/headers/print_visitor.h"
#include "./visitors/headers/register_allocator.h"
#include "./visitors/headers/grad_visitor.h"

int main()
{
    std::shared_ptr<Node> x = std::make_shared<Variable>(1.0);
    std::shared_ptr<Node> y = std::make_shared<Variable>(2.0);
    std::shared_ptr<Node> c = std::make_shared<Const>(3.0);
    std::shared_ptr<Node> expr = std::make_shared<Add>(x, std::make_shared<Multiply>(c, x));

    // Create the PrintVisitor
    PrintVisitor printer;
    GradVisitor gv;
    RegisterAllocator ra;
    
    GradientMap gm = gv.backward(expr);
    RegisterMap rm = ra.allocate_registers(expr);
    
    expr->accept(&printer);
    std::cout << std::endl;
    gm[x.get()]->accept(&printer);
    std::cout << std::endl;
}