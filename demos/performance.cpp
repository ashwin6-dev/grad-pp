#include "./headers/performance.h"

void performance() {
    // Define a vector to hold different computation graphs
    std::vector<std::shared_ptr<Node>> graphs;

    // Create different computation graphs
    std::shared_ptr<Variable> w1 = make_variable(10.0);
    std::shared_ptr<Variable> b1 = make_variable(5.0);
    graphs.push_back(make_divide(make_add(b1, make_multiply(make_add(w1, b1), b1)), w1));

    std::shared_ptr<Variable> w2 = make_variable(2.0);
    std::shared_ptr<Variable> b2 = make_variable(3.0);
    graphs.push_back(make_multiply(make_add(w2, b2), make_subtract(w1, b2)));

    std::shared_ptr<Variable> w3 = make_variable(1.0);
    std::shared_ptr<Variable> b3 = make_variable(4.0);
    graphs.push_back(make_add(make_multiply(w3, b3), make_divide(make_const(20.0), w1)));

    std::shared_ptr<Variable> w4 = make_variable(7.0);
    std::shared_ptr<Variable> b4 = make_variable(2.0);
    graphs.push_back(make_multiply(make_add(w4, b4), make_multiply(make_const(10.0), w4)));

    RegisterAllocator ra;
    Emitter emitter;
    JITVisitor jit(ra, emitter);
    EvaluationVisitor ev;
    PrintVisitor pv;

    const int iterations = 10000;

    // Loop through each graph and perform performance testing
    for (size_t i = 0; i < graphs.size(); ++i) {
        std::shared_ptr<Node> y = graphs[i];

        // JIT-Compiled Execution Timing
        auto start_jit = std::chrono::high_resolution_clock::now();
        compiled jit_func = jit.jit(y);
        
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

        // Output results for the current graph
        std::cout << "Graph: ";
        y->accept(&pv);
        std::cout << std::endl;
        std::cout << "JIT-Compiled result: " << result_jit << std::endl;
        std::cout << "JIT-Compiled execution time: " << jit_duration.count() << " seconds" << std::endl;
        std::cout << "Non-JIT (interpreted) result: " << result_nonjit << std::endl;
        std::cout << "Non-JIT execution time: " << nonjit_duration.count() << " seconds" << std::endl;
        std::cout << "Performance ratio (non-JIT/JIT): " 
                  << (nonjit_duration.count() / jit_duration.count()) 
                  << "x slower in interpreted mode\n\n";
    }
}
