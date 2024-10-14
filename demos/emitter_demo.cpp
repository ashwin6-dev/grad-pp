#include "headers/emitter_demo.h"

void emitter_demo()
{
    Emitter emitter;
    emitter.movsd_imm_to_xmm(100.0, 0); // move 100 to xmm0
    emitter.movesd_memory_reg(1, 0); // move [rdi] to xmm1 (first argument is register, second argument is displacement from [rdi])
    emitter.mulsd(1, 0); // multiply xmm1 and xmm0 and store it in xmm0

    compiled func = emitter.compile();

    double inputs[] = { 0.0 }; // address of input array is stored in rdi

    for (int i = 0; i <= 10; i++) {
        inputs[0] = i;
        std::cout << func(inputs) << std::endl;
    }
}
