#include "./print-visitor/headers/print_visitor.h"
#include "./register-allocator/headers/register_allocator.h"
#include "./grad-visitor/headers/grad_visitor.h"
#include "./jit/headers/jit.h"

int main()
{
    std::shared_ptr<Node> x = make_input();
    std::shared_ptr<Node> w = make_input();
    std::shared_ptr<Node> y = make_const(20.5);
    std::shared_ptr<Node> z = make_multiply(make_add(x, y), w);

    RegisterAllocator ra;
    Emitter emitter;
    JITVisitor jit(ra, emitter);

    compiled fn = jit.jit(z);

    double inputs[] = { 5.0, 10.0 };
    double res = fn(inputs);
    
    std::cout << res << std::endl;
}