#include <iostream>
#include <chrono>
#include "./print-visitor/headers/print_visitor.h"
#include "./register-allocator/headers/register_allocator.h"
#include "./grad-visitor/headers/grad_visitor.h"
#include "./jit/headers/jit.h"
#include "./eval-visitor/headers/eval_visitor.h"

std::shared_ptr<Node> build_model_graph(std::shared_ptr<Node> input, std::shared_ptr<Node> w, std::shared_ptr<Node> b)
{
    return make_add(make_multiply(input, w), b);
}

int main()
{
    std::shared_ptr<Variable> w = make_variable(10.0);
    std::shared_ptr<Variable> b = make_variable(5.0);
    std::shared_ptr<Node> y = make_subtract(b, make_multiply(make_add(w, b), b));

    RegisterAllocator ra;
    Emitter emitter;
    JITVisitor jit(ra, emitter);
    EvaluationVisitor ev;

    // JIT-Compiled Execution Timing
    auto start_jit = std::chrono::high_resolution_clock::now();
    compiled func = jit.jit(y);
    
    const int iterations = 10000;
    double result_jit = 0;
    for (int i = 0; i < iterations; ++i) {
        result_jit += func(nullptr);
    }
    auto end_jit = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> jit_duration = end_jit - start_jit;
    std::cout << "JIT-Compiled result: " << result_jit << std::endl;
    std::cout << "JIT-Compiled execution time: " << jit_duration.count() << " seconds" << std::endl;

    // Non-JIT (Interpreted) Execution Timing
    auto start_nonjit = std::chrono::high_resolution_clock::now();
    double result_nonjit = 0;
    for (int i = 0; i < iterations; ++i) {
        result_nonjit += ev.evaluate(y, nullptr);
    }
    auto end_nonjit = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> nonjit_duration = end_nonjit - start_nonjit;
    std::cout << "Non-JIT (interpreted) result: " << result_nonjit << std::endl;
    std::cout << "Non-JIT execution time: " << nonjit_duration.count() << " seconds" << std::endl;

    // Comparison of JIT vs Non-JIT
    std::cout << "Performance ratio (non-JIT/JIT): " 
              << (nonjit_duration.count() / jit_duration.count()) 
              << "x slower in interpreted mode" << std::endl;

    return 0;
}
