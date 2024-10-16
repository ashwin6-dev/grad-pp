#pragma once

#include <vector>

typedef double (*compiled)(double*);

class Emitter {
    std::vector<int> instructions; // Changed from char to int

public:
    void movesd_imm_to_reg(double value, int reg);         // Changed char to int
    void movesd_reg_rdi(int reg, int displacement);     // Changed char to int
    void movesd_rdi_reg(int reg, int displacement);     // Changed char to int
    void movesd_reg_reg(int src, int dest);                // Changed char to int
    void movesd_pointer_reg(void* address, int reg);
    void movesd_reg_pointer(int reg, void* address);
    void mulsd(int src, int dest);                          // Changed char to int
    void addsd(int src, int dest);                          // Changed char to int
    void subsd(int src, int dest);                          // Changed char to int
    void divsd(int src, int dest);                          // Changed char to int
    void push_xmm(int reg);
    void pop_xmm(int reg);
    void clear();
    compiled compile();
};
