#include "./headers/deriv.h"

void deriv()
{
    std::shared_ptr<Variable> x = make_variable(10.0);
    std::shared_ptr<Variable> y = make_variable(5.0);

    // 5 * (x*x + y*x)
    std::shared_ptr<Node> graph = make_multiply(
        make_add(
            make_multiply(x, x),
            make_multiply(y, x)
        ),
        make_const(5.0)
    );

    GradVisitor g = make_grad_visitor();
    GradientMap gradients = g.backward(graph);

    std::shared_ptr<Node> x_grad = gradients[x.get()]; // should simplify to 10x + 5y
    std::shared_ptr<Node> y_grad = gradients[y.get()]; // should simplify to 5x

    EvaluationVisitor ev = make_evaluation_visitor();
    double inputs[] = {};

    std::cout << ev.evaluate(x_grad, inputs) << std::endl; // should be 10x + 5y = 125
    std::cout << ev.evaluate(y_grad, inputs) << std::endl; // should be 5x = 50

    DotVisitor dv;

    std::unordered_map<Node*, std::string> var_names = {
        {x.get(), "x"},
        {y.get(), "y"}
    };
    
    std::string graph_dot = dv.get_dot_representation(graph, var_names);
    std::string x_grad_dot = dv.get_dot_representation(x_grad, var_names);
    std::string y_grad_dot = dv.get_dot_representation(y_grad, var_names);

    write_dot_file("computation_graph.dot", graph_dot);
    write_dot_file("x_gradient.dot", x_grad_dot);
    write_dot_file("y_gradient.dot", y_grad_dot);

    generate_image_from_dot("computation_graph.dot", "computation_graph.png");
    generate_image_from_dot("x_gradient.dot", "x_gradient.png");
    generate_image_from_dot("y_gradient.dot", "y_gradient.png");
}