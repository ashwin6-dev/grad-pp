#pragma once

#include <vector>

typedef double (*compiled)(double*);

class Emitter {
    std::vector<int> instructions; // Changed from char to int

public:
    void movsd_imm_to_xmm(double value, int reg);         // Changed char to int
    void movesd_reg_memory(int reg, int displacement);     // Changed char to int
    void movesd_memory_reg(int reg, int displacement);     // Changed char to int
    void movesd_reg_reg(int src, int dest);                // Changed char to int
    void movsd_pointer_xmm(void* address, int reg);
    void movsd_xmm_pointer(int reg, void* address);
    void mulsd(int src, int dest);                          // Changed char to int
    void addsd(int src, int dest);                          // Changed char to int
    void subsd(int src, int dest);                          // Changed char to int
    void divsd(int src, int dest);                          // Changed char to int
    void push_xmm(int reg);
    void pop_xmm(int reg);
    void clear();
    compiled compile();
};
