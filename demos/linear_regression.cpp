#include "./headers/performance.h"
#include "../dot-visitor/headers/dot_visitor.h"

void linear_regression()
{
    std::shared_ptr<Variable> w = make_variable(5.0);
    std::shared_ptr<Variable> b = make_variable(3.0);

    double inputs[] = { 1.0, 2.0 };
    double outputs[] = { 5.0, 9.0 };


    std::shared_ptr<Node> total_loss = make_const(0.0);

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        std::shared_ptr<Node> input = make_const(inputs[i]);
        std::shared_ptr<Node> model_output = make_add(make_multiply(w, input), b);
        std::shared_ptr<Node> error = make_subtract(model_output, make_const(outputs[i]));
        std::shared_ptr<Node> loss = make_multiply(error, error);
        total_loss = make_add(total_loss, loss);
    }

    RegisterAllocator ra;
    Emitter emitter;
    JITVisitor jit(ra, emitter);

    GradVisitor g;
    GradientMap gm = g.backward(total_loss);
    std::shared_ptr<Node> w_grad = gm[w.get()];
    std::shared_ptr<Node> b_grad = gm[b.get()];
    
    compiled loss_jit = jit.jit(total_loss);
    compiled w_grad_jit = jit.jit(w_grad);
    compiled b_grad_jit = jit.jit(b_grad);

    EvaluationVisitor ev;

    double lr = 0.01;

    DotVisitor dv;
    std::unordered_map<Node*, std::string> var_names = {
        { w.get(), "w" },
        { b.get(), "b" }
    };

    RegisterMap rm = ra.allocate_registers(w_grad);

    std::string graph_dot = dv.get_dot_reg_representation(w_grad, var_names, rm);
    
    write_dot_file("total_loss.dot", graph_dot);
    generate_image_from_dot("total_loss.dot", "total_loss.png");

    
    for (int epoch = 1; epoch <= 5; epoch++) {
        std::cout << "loss calc" << std::endl;
        double loss = loss_jit(nullptr);
        std::cout << "w calc" << std::endl;
        double w_delta = w_grad_jit(nullptr);
        std::cout << "b calc" << std::endl;
        double b_delta = b_grad_jit(nullptr);

        std::cout << "loss " << loss << " real loss " << ev.evaluate(total_loss, nullptr) << std::endl;
        std::cout << "b_delta " << b_delta << " real b_delta " << ev.evaluate(b_grad, nullptr) << std::endl;
        std::cout << "w_delta " << w_delta << " real w_delta " << ev.evaluate(w_grad, nullptr) << std::endl;

        w->set_value(w->get_value() - lr * w_delta);
        b->set_value(b->get_value() - lr * b_delta);

        loss_jit = jit.jit(total_loss);
        w_grad_jit = jit.jit(w_grad);
        b_grad_jit = jit.jit(b_grad);
    }
}