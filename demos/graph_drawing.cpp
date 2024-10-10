#include "./headers/graph_drawing.h"

void draw_graphs(){
    std::shared_ptr<Variable> x = make_variable(10.0);
    std::shared_ptr<Variable> y = make_variable(5.0);
    std::shared_ptr<Node> graph = make_multiply(
        make_add(
            make_multiply(make_const(5.0), x),
            make_multiply(make_const(10.0), x)
        ),
        y
    );


    std::unordered_map<Node*, std::string> var_names = {
        {x.get(), "x"},
        {y.get(), "y"}
    };

    GradVisitor g;
    GradientMap m = g.backward(graph);

    std::shared_ptr<Node> x_grad = m[x.get()];
    std::shared_ptr<Node> y_grad = m[y.get()];

    // Create DotVisitor instance to get DOT representation
    DotVisitor dv;

    // Get DOT representations
    std::string graph_dot = dv.get_dot_representation(graph, var_names);
    std::string x_grad_dot = dv.get_dot_representation(x_grad, var_names);
    std::string y_grad_dot = dv.get_dot_representation(y_grad, var_names);

    // Write DOT files
    write_dot_file("computation_graph.dot", graph_dot);
    write_dot_file("x_gradient.dot", x_grad_dot);
    write_dot_file("y_gradient.dot", y_grad_dot);

    // Generate images from DOT files
    generate_image_from_dot("computation_graph.dot", "computation_graph.png");
    generate_image_from_dot("x_gradient.dot", "x_gradient.png");
    generate_image_from_dot("y_gradient.dot", "y_gradient.png");

    std::cout << "DOT files and images generated successfully!" << std::endl;
}