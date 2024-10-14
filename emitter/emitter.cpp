#include "headers/emitter.h"
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <stdint.h>

void Emitter::clear()
{
    instructions.clear();
}

void Emitter::movsd_imm_to_xmm(double value, int reg) 
{
    double* value_ptr = new double(value);
    movsd_pointer_xmm(value_ptr, reg);
}

void Emitter::movesd_reg_memory(int reg, int displacement)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x11;

    uint8_t rex = 0x40; // Base REX prefix for using XMM registers

    // Set the REX.R bit if reg is 8 or greater
    if (reg >= 8) {
        rex |= 0x4; // Set REX.R bit for XMM8 and above
        reg -= 8;   // Adjust reg value to fit in ModR/M
    }

    int modrm = 0x47 | (reg << 3); // 87 means [rdi + disp8]

    instructions.push_back(rex);
    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
    instructions.push_back(displacement);
}

void Emitter::movesd_memory_reg(int reg, int displacement)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x10;

    uint8_t rex = 0x40; // Base REX prefix for using XMM registers

    // Set the REX.R bit if reg is 8 or greater
    if (reg >= 8) {
        rex |= 0x4; // Set REX.R bit for XMM8 and above
        reg -= 8;   // Adjust reg value to fit in ModR/M
    }

    int modrm = 0x47 | (reg << 3); // 87 means [rdi + disp8]

    instructions.push_back(rex);
    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
    instructions.push_back(displacement);
}

void Emitter::movsd_pointer_xmm(void* address, int reg) {
    instructions.push_back(0x48);          // REX.W prefix for 64-bit operation
    instructions.push_back(0xbe);          // mov rsi, imm64 opcode (mov rsi, <address>)
    long long addr = reinterpret_cast<long long>(address); // Get the address of the double value

    for (int i = 0; i < 8; ++i) {
        instructions.push_back((addr >> (i * 8)) & 0xff); // Push 8 bytes of the address
    }

    int prefix = 0xf2;                    // Operand-size prefix for MOVSD (SSE2)
    int opcode_prefix = 0x0f;             // Opcode prefix for SIMD instructions
    int opcode = 0x10;                    // Opcode for movsd (memory to register)
    
    uint8_t rex = 0x40; // Base REX prefix for using XMM registers

    // Set the REX.R bit if reg is 8 or greater
    if (reg >= 8) {
        rex |= 0x4; // Set REX.R bit for destination register (XMM8 and above)
        reg -= 8;   // Adjust reg value to fit in the ModR/M byte (reg bits are 3-bit wide)
    }

    int modrm = 0x06 | (reg << 3);        // ModR/M byte (00 | reg << 3 | RSI)

    // Add the MOVSD instruction to the instruction stream
    instructions.push_back(rex);           // REX prefix
    instructions.push_back(prefix);        // f2 prefix for MOVSD
    instructions.push_back(opcode_prefix); // 0f prefix
    instructions.push_back(opcode);        // Opcode 10 for MOVSD xmm<reg>, [rsi]
    instructions.push_back(modrm);         // ModR/M byte
}

void Emitter::movesd_reg_reg(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x11;

    uint8_t rex = 0x40;
    if (dest >= 8) {
        rex |= 0x1; // Set REX.B bit for destination XMM8 and above
        dest -= 8;
    }
    
    if (src >= 8) {
        rex |= 0x4; // Set REX.R bit for source XMM8 and above
        src -= 8;
    }

    instructions.push_back(rex);

    int modrm = 0xc0 | (src << 3) | dest; // mod = 11 (register-to-register)

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::addsd(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x58;

    uint8_t rex = 0x40;
    if (dest >= 8) {
        rex |= 0x1; // Set REX.B bit for destination XMM8 and above
        dest -= 8;
    }
    
    if (src >= 8) {
        rex |= 0x4; // Set REX.R bit for source XMM8 and above
        src -= 8;
    }

    instructions.push_back(rex);

    int modrm = 0xc0 | (dest << 3) | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::subsd(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x5c;

    uint8_t rex = 0x40;
    if (dest >= 8) {
        rex |= 0x1; // Set REX.B bit for destination XMM8 and above
        dest -= 8;
    }
    
    if (src >= 8) {
        rex |= 0x4; // Set REX.R bit for source XMM8 and above
        src -= 8;
    }

    instructions.push_back(rex);

    int modrm = 0xc0 | (dest << 3) | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::mulsd(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x59;

    uint8_t rex = 0x40;
    if (dest >= 8) {
        rex |= 0x1; // Set REX.B bit for destination XMM8 and above
        dest -= 8;
    }
    
    if (src >= 8) {
        rex |= 0x4; // Set REX.R bit for source XMM8 and above
        src -= 8;
    }

    instructions.push_back(rex);

    int modrm = 0xc0 | (dest << 3) | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::divsd(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x5e;

    uint8_t rex = 0x40;
    if (dest >= 8) {
        rex |= 0x1; // Set REX.B bit for destination XMM8 and above
        dest -= 8;
    }
    
    if (src >= 8) {
        rex |= 0x4; // Set REX.R bit for source XMM8 and above
        src -= 8;
    }

    instructions.push_back(rex);

    int modrm = 0xc0 | (dest << 3) | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::push_xmm(int reg)
{
    // Adjust the stack pointer to make space for one XMM register (16 bytes)
    instructions.push_back(0x48);            // REX.W prefix for 64-bit operation
    instructions.push_back(0x83);            // Opcode for SUB instruction
    instructions.push_back(0xec);            // ModRM: sub rsp, 16
    instructions.push_back(0x10);            // 0x10 for 16 bytes (SSE)
    
    // Move the value of the XMM register to the stack using MOVDQU (unaligned move)
    if (reg >= 8) {
        instructions.push_back(0x44);        // REX.R prefix for XMM8-XMM15
    }
    instructions.push_back(0xf3);            // Prefix for MOVDQU
    instructions.push_back(0x0f);            // Opcode prefix
    instructions.push_back(0x7f);            // Opcode for MOVDQU (XMM to memory)
    instructions.push_back(0x04 | ((reg & 7) << 3)); // ModRM byte
    instructions.push_back(0x24);            // SIB byte for [rsp]
}

void Emitter::pop_xmm(int reg)
{
    // Move the value from the stack into the XMM register using MOVDQU (unaligned move)
    if (reg >= 8) {
        instructions.push_back(0x44);        // REX.R prefix for XMM8-XMM15
    }
    instructions.push_back(0xf3);            // Prefix for MOVDQU
    instructions.push_back(0x0f);            // Opcode prefix
    instructions.push_back(0x6f);            // Opcode for MOVDQU (memory to XMM)
    instructions.push_back(0x04 | ((reg & 7) << 3)); // ModRM byte
    instructions.push_back(0x24);            // SIB byte for [rsp]

    // Adjust the stack pointer to remove the XMM register from the stack
    instructions.push_back(0x48);            // REX.W prefix for 64-bit operation
    instructions.push_back(0x83);            // Opcode for ADD instruction
    instructions.push_back(0xc4);            // ModRM: add rsp, 16
    instructions.push_back(0x10);            // 0x10 for 16 bytes (SSE)
}

compiled Emitter::compile()
{
    int size = instructions.size() + 1;

    void* mem = mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (mem == MAP_FAILED) {
        perror("mmap");
        return nullptr;
    }

    char* code = (char*) malloc(size);

    for (int i = 0; i < size - 1; i++) {
        code[i] = instructions[i];
    }

    code[size - 1] = 0xc3; // Return instruction (ret)

    std::memcpy(mem, code, size);

    free(code); // Free allocated memory for code buffer

    return (compiled) mem;
}
