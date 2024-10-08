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
    std::shared_ptr<Variable> b = make_variable(1.0);

    RegisterAllocator ra;
    Emitter emitter;
    JITVisitor jit(ra, emitter);

    double inputs[] = { 1.0 };
    double outputs[] = { 3.0 };

    std::shared_ptr<Node> total_loss = make_const(0);

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        std::shared_ptr<Node> model_graph = build_model_graph(make_const(inputs[i]), w, b);
        std::shared_ptr<Node> error = make_subtract(model_graph, make_const(outputs[i]));
        std::shared_ptr<Node> loss = make_multiply(error, error);

        total_loss = make_add(loss, total_loss);
    }

    GradVisitor grad_visitor;
    GradientMap grad_map = grad_visitor.backward(total_loss);
    std::shared_ptr<Node> w_grad = grad_map[w.get()];
    std::shared_ptr<Node> b_grad = grad_map[b.get()];

    compiled loss_jit = jit.jit(total_loss);
    compiled b_jit = jit.jit(b_grad);
    compiled w_jit = jit.jit(w_grad);

    PrintVisitor pv;
    w_grad->accept(&pv);
    std::cout << std::endl;

    EvaluationVisitor ev;
    for (int epoch = 0; epoch < 1; epoch++) {
        double loss = loss_jit(nullptr);
        double wg = w_jit(nullptr);
        double bg = b_jit(nullptr);

        std::cout << wg << " " << ev.evaluate(w_grad, nullptr) << std::endl;
        std::cout << "loss " << loss << std::endl;

        w->set_value(w->get_value() - 0.01 * wg);
        b->set_value(b->get_value() - 0.01 * bg);

        loss_jit = jit.jit(total_loss);
        w_jit = jit.jit(w_grad);
        b_jit = jit.jit(b_grad);
    }
}