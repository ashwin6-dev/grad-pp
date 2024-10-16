#include "./headers/minimize_function.h"

void minimize_function()
{
    // Step 1: Define the variables x, y, z
    std::shared_ptr<Variable> x = make_variable(0);  // Initial value of x
    std::shared_ptr<Variable> y = make_variable(0);  // Initial value of y
    std::shared_ptr<Variable> z = make_variable(0);  // Initial value of z

    // Step 2: Define the complex function f(x, y, z) = (x-2)^2 + (y-4)(y-2) + (z+1)^2
    std::shared_ptr<Node> f_x_y_z = make_add(
        make_add(
            make_multiply(
                make_subtract(x, make_const(2.0)),
                make_subtract(x, make_const(2.0))),
            make_multiply(
                make_subtract(y, make_const(4.0)),
                make_subtract(y, make_const(2.0)))
        ),
        make_multiply(
            make_add(z, make_const(1.0)),
            make_add(z, make_const(1.0))));

    // Step 3: Compute the gradients of f with respect to x, y, z
    GradVisitor g;
    GradientMap gm = g.backward(f_x_y_z);

    std::shared_ptr<Node> x_grad = gm[x.get()]; // Gradient of f with respect to x
    std::shared_ptr<Node> y_grad = gm[y.get()]; // Gradient of f with respect to y
    std::shared_ptr<Node> z_grad = gm[z.get()]; // Gradient of f with respect to z

    // Step 4: Set up JIT and compilation for the function and its gradients
    RegisterAllocator ra;
    Emitter emitter;
    JITVisitor jit(ra, emitter);

    compiled f_jit = jit.jit(f_x_y_z);            // Compiled function f(x, y, z)
    compiled x_grad_jit = jit.jit(x_grad);        // Compiled gradient of f with respect to x
    compiled y_grad_jit = jit.jit(y_grad);        // Compiled gradient of f with respect to y
    compiled z_grad_jit = jit.jit(z_grad);        // Compiled gradient of f with respect to z

    EvaluationVisitor ev;

    // Step 5: Gradient descent parameters
    double lr = 0.05;  // Learning rate
    int epochs = 200;   // Number of iterations

    // Step 6: Perform gradient descent
    for (int epoch = 1; epoch <= epochs; epoch++) {
        std::cout << "Epoch " << epoch << std::endl;

        // Calculate current value of the function and gradients
        double f_value = f_jit(nullptr);
        double x_grad_value = x_grad_jit(nullptr);
        double y_grad_value = y_grad_jit(nullptr);
        double z_grad_value = z_grad_jit(nullptr);

        std::cout << "f(x, y, z): " << f_value << std::endl;
        std::cout << "Gradient of f(x, y, z) w.r.t x: " << x_grad_value << std::endl;
        std::cout << "Gradient of f(x, y, z) w.r.t y: " << y_grad_value << std::endl;
        std::cout << "Gradient of f(x, y, z) w.r.t z: " << z_grad_value << std::endl;

        // Update the values of x, y, z using gradient descent
        x->set_value(x->get_value() - lr * x_grad_value);
        y->set_value(y->get_value() - lr * y_grad_value);
        z->set_value(z->get_value() - lr * z_grad_value);

        std::cout << "Updated x: " << x->get_value() << std::endl;
        std::cout << "Updated y: " << y->get_value() << std::endl;
        std::cout << "Updated z: " << z->get_value() << std::endl;
    }
}
