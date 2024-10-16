#pragma once

#include <vector>

typedef double (*compiled)(double*);

class Emitter {
private:
    std::vector<int> instructions; // Changed from char to int
    int calculate_rex(int &src, int &dest);
    int calculate_rex(int &reg);
    void emit_instruction(int rex, int prefix, int opcode_prefix, int opcode, int modrm, int displacement = -1);

public:
    void movesd_imm_to_reg(double value, int reg);         // Changed char to int
    void movesd_reg_rdi(int reg, int displacement);     // Changed char to int
    void movesd_rdi_reg(int reg, int displacement);     // Changed char to int
    void movesd_reg_reg(int src, int dest);                // Changed char to int
    void movesd_pointer_reg(void* address, int reg);
    void movesd_reg_pointer(int reg, void* address);
    void perform_arithmetic(int src, int dest, int opcode);
    void mulsd(int src, int dest);                          // Changed char to int
    void addsd(int src, int dest);                          // Changed char to int
    void subsd(int src, int dest);                          // Changed char to int
    void divsd(int src, int dest);                          // Changed char to int
    void push_xmm(int reg);
    void pop_xmm(int reg);
    void clear();
    compiled compile();
};
