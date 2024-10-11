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

    instructions.push_back(0x48);          // REX.W prefix for 64-bit operation
    instructions.push_back(0xbe);          // mov rsi, imm64 opcode (mov rsi, <address>)
    long long addr = reinterpret_cast<long long>(value_ptr); // Get the address of the double value
    for (int i = 0; i < 8; ++i) {
        instructions.push_back((addr >> (i * 8)) & 0xff); // Push 8 bytes of the address
    }

    int prefix = 0xf2;                    // Operand-size prefix for MOVSD (SSE2)
    int opcode_prefix = 0x0f;             // Opcode prefix for SIMD instructions
    int opcode = 0x10;                    // Opcode for movsd (memory to register)
    
    // Check if using extended XMM register
    if (reg >= 8) {
        instructions.push_back(0x41); // REX prefix to use XMM8-XMM15
    }

    int modrm = 0x06 | (reg << 3);        // ModR/M byte (00 | reg << 3 | RSI)

    // Add the MOVSD instruction to the instruction stream
    instructions.push_back(prefix);        // f2 prefix for MOVSD
    instructions.push_back(opcode_prefix); // 0f prefix
    instructions.push_back(opcode);        // Opcode 10 for MOVSD xmm<reg>, [rsi]
    instructions.push_back(modrm);         // ModR/M byte
}

void Emitter::movesd_reg_memory(int reg, int displacement)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x11;

    // Check if using extended XMM register
    if (reg >= 8) {
        instructions.push_back(0x41); // REX prefix to use XMM8-XMM15
    }

    int mod = 0x47 | (reg << 3); // using the address at rdi + given displacement

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
    instructions.push_back(displacement);
}

void Emitter::movesd_memory_reg(int reg, int displacement)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x10;

    // Check if using extended XMM register
    if (reg >= 8) {
        instructions.push_back(0x41); // REX prefix to use XMM8-XMM15
    }

    int mod = 0x47 | (reg << 3); // using the address at rdi + given displacement

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
    instructions.push_back(displacement);
}

void Emitter::movsd_pointer_xmm(void* address, int reg)
{
    int prefix = 0xf2;           // Prefix for MOVSD
    int opcode_prefix = 0x0f;    // Common prefix for SIMD instructions
    int opcode = 0x10;           // MOVSD opcode (memory to XMM register)
    int modrm = 0x04 | (reg << 3); // ModR/M byte: mod = 00, reg = XMM reg, r/m = 100 (SIB follows)
    int sib = 0x25;              // SIB byte for absolute addressing

    // Check if using extended XMM register (XMM8-XMM15 requires REX prefix)
    if (reg >= 8) {
        instructions.push_back(0x44); // REX.R prefix to access extended registers
    }

    // Add instruction prefixes and opcode
    instructions.push_back(0x48);
    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
    instructions.push_back(sib);

    // Add the 64-bit absolute address
    uint64_t addr = reinterpret_cast<uint64_t>(address);
    for (int i = 0; i < 8; ++i) {
        instructions.push_back(0xff & (addr >> (i * 8)));
    }
}

void Emitter::movesd_reg_reg(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x11;

    // Check if using extended XMM registers
    if (src >= 8 || dest >= 8) {
        instructions.push_back(0x41); // REX prefix for extended registers
    }

    int mod = 0xc0 | (src << 3) | dest;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
}

void Emitter::addsd(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x58;

    // Check if using extended XMM registers
    if (src >= 8 || dest >= 8) {
        instructions.push_back(0x41); // REX prefix for extended registers
    }

    int mod = 0xc0 | (dest << 3) | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
}

void Emitter::subsd(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x5c;

    // Check if using extended XMM registers
    if (src >= 8 || dest >= 8) {
        instructions.push_back(0x41); // REX prefix for extended registers
    }

    int mod = 0xc0 | (dest << 3) | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
}

void Emitter::mulsd(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x59;

    // Check if using extended XMM registers
    if (src >= 8 || dest >= 8) {
        instructions.push_back(0x41); // REX prefix for extended registers
    }

    int mod = 0xc0 | (dest << 3) | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
}

void Emitter::divsd(int src, int dest)
{
    int prefix = 0xf2;
    int opcode_prefix = 0x0f;
    int opcode = 0x5e;

    // Check if using extended XMM registers
    if (src >= 8 || dest >= 8) {
        instructions.push_back(0x41); // REX prefix for extended registers
    }

    int mod = 0xc0 | (dest << 3) | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
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