#include "headers/emitter.h"
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <cstring>

void Emitter::movsd_imm_to_xmm(double value, char reg) 
{
    // 1. Allocate memory to store the immediate double value.
    // We'll use mmap to create a writable region for storing the double.
    char* temp_mem = (char*) mmap(nullptr, sizeof(double), PROT_READ | PROT_WRITE, 
                                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (temp_mem == MAP_FAILED) {
        perror("mmap");
        return;
    }

    // Store the immediate double value in the allocated memory.
    std::memcpy(temp_mem, &value, sizeof(double));

    // 2. Generate the machine instructions:
    // - Move the memory address of the temp location into a general-purpose register, like RSI.
    // - Use the `movsd` instruction to load the value from [RSI] into the XMM register.

    // Instruction: mov rsi, <temp_mem_address>
    instructions.push_back(0x48);          // REX.W prefix for 64-bit operation
    instructions.push_back(0xbe);          // mov rsi, imm64 opcode (mov rsi, <address>)
    long long addr = (long long) temp_mem; // Get the address of the temp memory
    for (int i = 0; i < 8; ++i) {
        instructions.push_back((addr >> (i * 8)) & 0xff); // Push 8 bytes of the address
    }

    // Instruction: movsd xmm<reg>, [rsi]
    // This moves the double value from memory at [RSI] to the XMM register.
    char prefix = 0xf2;                    // Operand-size prefix for MOVSD (SSE2)
    char opcode_prefix = 0x0f;             // Opcode prefix for SIMD instructions
    char opcode = 0x10;                    // Opcode for movsd (memory to register)
    char modrm = 0x06 | (reg << 3);        // ModR/M byte (00 | reg << 3 | RSI)

    // Add the MOVSD instruction to the instruction stream
    instructions.push_back(prefix);        // f2 prefix for MOVSD
    instructions.push_back(opcode_prefix); // 0f prefix
    instructions.push_back(opcode);        // Opcode 10 for MOVSD xmm<reg>, [rsi]
    instructions.push_back(modrm);         // ModR/M byte
}

void Emitter::movesd_reg_memory(char reg, char displacement)
{
    char prefix = 0xf2;
    char opcode_prefix = 0x0f;
    char opcode = 0x11;
    char mod = 0x47 | reg << 3; // using the address at rdi + given displacement

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
    instructions.push_back(displacement);
}

void Emitter::movesd_memory_reg(char reg, char displacement)
{
    char prefix = 0xf2;
    char opcode_prefix = 0x0f;
    char opcode = 0x10;
    char mod = 0x47 | reg << 3; // using the address at rdi + given displacement

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
    instructions.push_back(displacement);
}

void Emitter::movesd_reg_reg(char src, char dest)
{
    char prefix = 0xf2;
    char opcode_prefix = 0x0f;
    char opcode = 0x11;
    char mod = 0xc0 | src << 3 | dest;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
}

void Emitter::addsd(char src, char dest)
{
    char prefix = 0xf2;
    char opcode_prefix = 0x0f;
    char opcode = 0x58;
    char mod = 0xc0 | dest << 3 | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
}

void Emitter::subsd(char src, char dest)
{
    char prefix = 0xf2;
    char opcode_prefix = 0x0f;
    char opcode = 0x5c;
    char mod = 0xc0 | dest << 3 | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
}

void Emitter::mulsd(char src, char dest)
{
    char prefix = 0xf2;
    char opcode_prefix = 0x0f;
    char opcode = 0x59;
    char mod = 0xc0 | dest << 3 | src;

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(mod);
}

void Emitter::divsd(char src, char dest)
{
    char prefix = 0xf2;
    char opcode_prefix = 0x0f;
    char opcode = 0x5e;
    char mod = 0xc0 | dest << 3 | src;

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

    code[size - 1] = 0xc3;

    std::memcpy(mem, code, size);

    return (compiled) mem;
}