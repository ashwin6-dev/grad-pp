#include "./print-visitor/headers/print_visitor.h"
#include "./register-allocator/headers/register_allocator.h"
#include "./grad-visitor/headers/grad_visitor.h"

int main()
{
    std::shared_ptr<Node> x = std::make_shared<Input>();
    std::shared_ptr<Node> y = std::make_shared<Variable>(2.0);
    std::shared_ptr<Node> c = std::make_shared<Const>(3.0);
    std::shared_ptr<Node> expr = std::make_shared<Add>(x, std::make_shared<Multiply>(c, x));

    // Create the PrintVisitor
    PrintVisitor printer;
    GradVisitor gv;
    
    GradientMap gm = gv.backward(expr);
    
    expr->accept(&printer);
    std::cout << std::endl;

    gm[x.get()]->accept(&printer);
    std::cout << std::endl;
}