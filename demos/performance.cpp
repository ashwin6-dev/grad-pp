#include "./headers/performance.h"

int main() {
    std::vector<std::shared_ptr<Node>> graphs;

    std::shared_ptr<Variable> w1 = make_variable(10.0);
    std::shared_ptr<Variable> b1 = make_variable(5.0);
    graphs.push_back(make_divide(make_add(b1, make_multiply(make_add(w1, b1), b1)), w1));

    std::shared_ptr<Variable> w2 = make_variable(2.0);
    std::shared_ptr<Variable> b2 = make_variable(4.0);
    graphs.push_back(make_multiply(make_add(w2, b2), make_subtract(w1, b2)));

    std::shared_ptr<Variable> w3 = make_variable(1.0);
    std::shared_ptr<Variable> b3 = make_variable(4.0);
    graphs.push_back(make_add(make_multiply(w3, b3), make_divide(make_const(20.0), w1)));

    std::shared_ptr<Variable> w4 = make_variable(7.0);
    std::shared_ptr<Variable> b4 = make_variable(2.0);
    graphs.push_back(make_multiply(make_add(w4, b4), make_multiply(make_const(10.0), w4)));

    std::shared_ptr<Variable> w5 = make_variable(10.0);
    std::shared_ptr<Variable> b5 = make_variable(12.0);
    graphs.push_back(make_add(b5, make_multiply(w5, make_add(w5, make_add(w5, make_add(w5, b5))))));

    RegisterAllocator ra;
    Emitter emitter;
    JITVisitor jit(ra, emitter);
    EvaluationVisitor ev;
    PrintVisitor pv;
    
    const int iterations = 10000;
    for (size_t i = 0; i < graphs.size(); ++i) {
        std::shared_ptr<Node> y = graphs[i];

        // JIT-Compiled Execution Timing
        compiled jit_func = jit.jit(y);
        
        auto start_jit = std::chrono::high_resolution_clock::now();
        double result_jit = 0;
        for (int j = 0; j < iterations; ++j) {
            result_jit += jit_func(nullptr);
        }
        auto end_jit = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> jit_duration = end_jit - start_jit;

        // Non-JIT (Interpreted) Execution Timing
        auto start_nonjit = std::chrono::high_resolution_clock::now();
        double result_nonjit = 0;
        for (int j = 0; j < iterations; ++j) {
            result_nonjit += ev.evaluate(y, nullptr);
        }
        auto end_nonjit = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> nonjit_duration = end_nonjit - start_nonjit;

        std::cout << "Graph: ";
        y->accept(&pv);
        std::cout << std::endl;
        std::cout << "JIT-Compiled result: " << result_jit << std::endl;
        std::cout << "JIT-Compiled execution time: " << jit_duration.count() << " seconds" << std::endl;
        std::cout << "Non-JIT (interpreted) result: " << result_nonjit << std::endl;
        std::cout << "Non-JIT execution time: " << nonjit_duration.count() << " seconds" << std::endl;
        std::cout << "Performance ratio (non-JIT/JIT): " 
                  << (nonjit_duration.count() / jit_duration.count()) 
                  << "x faster in JIT mode\n\n";
    }
}
