#include "./headers/graph_drawing.h"

int main() {
    std::shared_ptr<Variable> x = make_variable(10.0);
    std::shared_ptr<Variable> y = make_variable(5.0);
    std::shared_ptr<Node> graph = make_multiply(
        make_add(
            make_multiply(make_const(5.0), x),
            make_multiply(make_const(10.0), x)
        ),
        y
    );

    RegisterAllocator allocator;
    RegisterMap allocation = allocator.allocate_registers(graph);

    // Create DotVisitor instance to get DOT representation
    DotVisitor dv;
    
    std::unordered_map<Node*, std::string> var_names = {
        {x.get(), "x"},
        {y.get(), "y"}
    };

    // Get DOT representations
    std::string graph_dot = dv.get_dot_reg_representation(graph, var_names, allocation);

    // Write DOT files
    write_dot_file("reg_alloc.dot", graph_dot);

    // Generate images from DOT files
    generate_image_from_dot("reg_alloc.dot", "reg_alloc.png");

    std::cout << "DOT files and images generated successfully!" << std::endl;
}