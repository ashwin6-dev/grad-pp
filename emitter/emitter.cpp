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
    int prefix = 0xf2;       // Prefix for double-precision floating-point operations
    int opcode_prefix = 0x0f; // Two-byte opcode prefix 
    int opcode = 0x11;       // Opcode for MOVSD (move scalar double-precision)

    int rex = 0x40; // Base REX prefix for using XMM registers

    // Set the REX.R bit if reg is 8 or greater
    if (reg >= 8) {
        rex |= 0x4; // Set REX.R bit for XMM8 and above
        reg -= 8;   // Adjust reg value to fit in ModR/M
    }

    int modrm = 0x47 | (reg << 3); // 0x47 means [rdi + disp8], reg in bits 3-5

    instructions.push_back(rex);
    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
    instructions.push_back(displacement);  // 8-bit displacement
}

void Emitter::movesd_memory_reg(int reg, int displacement)
{
    int prefix = 0xf2;       // Prefix for double-precision floating-point operations
    int opcode_prefix = 0x0f; // Two-byte opcode prefix
    int opcode = 0x10;       // Opcode for MOVSD (move scalar double-precision from memory)

    int rex = 0x40; // Base REX prefix for using XMM registers

    // Set the REX.R bit if reg is 8 or greater
    if (reg >= 8) {
        rex |= 0x4; // Set REX.R bit for XMM8 and above
        reg -= 8;   // Adjust reg value to fit in ModR/M
    }

    int modrm = 0x47 | (reg << 3); // 0x47 means [rdi + disp8], reg in bits 3-5

    instructions.push_back(rex);
    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
    instructions.push_back(displacement);  // 8-bit displacement
}

void Emitter::movsd_pointer_xmm(void* address, int reg) {
    // Moving value at pointer into reg (mov address to rsi, mov rsi to xmm)

    instructions.push_back(0x48);          // REX.W prefix for 64-bit operation
    instructions.push_back(0xbe);          // mov rsi, imm64 opcode (mov rsi, <address>)
    long long addr = reinterpret_cast<long long>(address);

    // Push 8 bytes of the address (little-endian order)
    for (int i = 0; i < 8; ++i) {
        instructions.push_back((addr >> (i * 8)) & 0xff);
    }

    int prefix = 0xf2;        // Operand-size prefix for MOVSD (SSE2)
    int opcode_prefix = 0x0f; // Opcode prefix for SIMD instructions
    int opcode = 0x10;        // Opcode for movsd (memory to register)
    
    int rex = 0x40; // Base REX prefix for using XMM registers

    if (reg >= 8) {
        rex |= 0x4; // Set REX.R bit for destination register (XMM8 and above)
        reg -= 8;   // Adjust reg value to fit in the ModR/M byte
    }

    int modrm = 0x06 | (reg << 3);  // ModR/M byte (00 | reg << 3 | RSI)

    instructions.push_back(rex);
    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::movsd_xmm_pointer(int reg, void* address) {
    instructions.push_back(0x48);          // REX.W prefix for 64-bit operation
    instructions.push_back(0xbe);          // mov rsi, imm64 opcode (mov rsi, <address>)
    long long addr = reinterpret_cast<long long>(address);

    // Push 8 bytes of the address (little-endian order)
    for (int i = 0; i < 8; ++i) {
        instructions.push_back((addr >> (i * 8)) & 0xff);
    }

    int prefix = 0xf2;        // Operand-size prefix for MOVSD (SSE2)
    int opcode_prefix = 0x0f; // Opcode prefix for SIMD instructions
    int opcode = 0x11;        // Opcode for movsd (register to memory)
    
    int rex = 0x40; // Base REX prefix for using XMM registers

    if (reg >= 8) {
        rex |= 0x4; // Set REX.R bit for source register (XMM8 and above)
        reg -= 8;   // Adjust reg value to fit in the ModR/M byte
    }

    int modrm = 0x06 | (reg << 3);  // ModR/M byte (00 | reg << 3 | RSI)

    instructions.push_back(rex);
    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::movesd_reg_reg(int src, int dest)
{
    int prefix = 0xf2;        // Prefix for double-precision floating-point operations
    int opcode_prefix = 0x0f; // Two-byte opcode prefix
    int opcode = 0x11;        // Opcode for MOVSD

    int rex = 0x40; // Base REX prefix
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
    int prefix = 0xf2;        // Prefix for double-precision floating-point operations
    int opcode_prefix = 0x0f; // Two-byte opcode prefix
    int opcode = 0x58;        // Opcode for ADDSD

    int rex = 0x40; // Base REX prefix
    if (dest >= 8) {
        rex |= 0x1; // Set REX.B bit for destination XMM8 and above
        dest -= 8;
    }
    
    if (src >= 8) {
        rex |= 0x4; // Set REX.R bit for source XMM8 and above
        src -= 8;
    }

    instructions.push_back(rex);

    int modrm = 0xc0 | (dest << 3) | src; // mod = 11 (register-to-register)

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::subsd(int src, int dest)
{
    int prefix = 0xf2;        // Prefix for double-precision floating-point operations
    int opcode_prefix = 0x0f; // Two-byte opcode prefix
    int opcode = 0x5c;        // Opcode for SUBSD

    int rex = 0x40; // Base REX prefix
    if (dest >= 8) {
        rex |= 0x1; // Set REX.B bit for destination XMM8 and above
        dest -= 8;
    }
    
    if (src >= 8) {
        rex |= 0x4; // Set REX.R bit for source XMM8 and above
        src -= 8;
    }

    instructions.push_back(rex);

    int modrm = 0xc0 | (dest << 3) | src; // mod = 11 (register-to-register)

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::mulsd(int src, int dest)
{
    int prefix = 0xf2;        // Prefix for double-precision floating-point operations
    int opcode_prefix = 0x0f; // Two-byte opcode prefix
    int opcode = 0x59;        // Opcode for MULSD

    int rex = 0x40; // Base REX prefix
    if (dest >= 8) {
        rex |= 0x1; // Set REX.B bit for destination XMM8 and above
        dest -= 8;
    }
    
    if (src >= 8) {
        rex |= 0x4; // Set REX.R bit for source XMM8 and above
        src -= 8;
    }

    instructions.push_back(rex);

    int modrm = 0xc0 | (dest << 3) | src; // mod = 11 (register-to-register)

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::divsd(int src, int dest)
{
    int prefix = 0xf2;        // Prefix for double-precision floating-point operations
    int opcode_prefix = 0x0f; // Two-byte opcode prefix
    int opcode = 0x5e;        // Opcode for DIVSD

    int rex = 0x40; // Base REX prefix
    if (dest >= 8) {
        rex |= 0x1; // Set REX.B bit for destination XMM8 and above
        dest -= 8;
    }
    
    if (src >= 8) {
        rex |= 0x4; // Set REX.R bit for source XMM8 and above
        src -= 8;
    }

    instructions.push_back(rex);

    int modrm = 0xc0 | (dest << 3) | src; // mod = 11 (register-to-register)

    instructions.push_back(prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(opcode);
    instructions.push_back(modrm);
}

void Emitter::push_xmm(int reg)
{
    int rex_w = 0x48;        // REX.W prefix for 64-bit operation
    int sub_opcode = 0x83;   // Opcode for SUB instruction
    int sub_modrm = 0xec;    // ModRM byte: SUB rsp, imm8
    int sub_imm = 0x10;      // Immediate value 16 (bytes to subtract from rsp)
    
    int movdqu_prefix = 0xf3;    // Prefix for unaligned MOVDQU instruction
    int opcode_prefix = 0x0f;    // Opcode prefix for SSE instructions
    int movdqu_store_opcode = 0x7f;  // Opcode for MOVDQU (store XMM to memory)
    
    // Subtract 16 bytes from rsp to make space for XMM register
    instructions.push_back(rex_w);
    instructions.push_back(sub_opcode);
    instructions.push_back(sub_modrm);
    instructions.push_back(sub_imm);
    
    int rex = 0x40;  // Base REX prefix
    if (reg >= 8) {
        rex |= 0x4; // Set REX.R bit for XMM8-XMM15
        reg -= 8;
    }
    
    if (rex != 0x40) {
        instructions.push_back(rex);
    }
    
    // MOVDQU instruction to store XMM register to [rsp]
    instructions.push_back(movdqu_prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(movdqu_store_opcode);
    
    int modrm = 0x04 | (reg << 3);  // ModRM byte: [rsp] as destination, reg as source
    int sib = 0x24;  // SIB byte for [rsp]
    
    instructions.push_back(modrm);
    instructions.push_back(sib);
}

void Emitter::pop_xmm(int reg)
{
    int movdqu_prefix = 0xf3;    // Prefix for unaligned MOVDQU instruction
    int opcode_prefix = 0x0f;    // Opcode prefix for SSE instructions
    int movdqu_load_opcode = 0x6f;   // Opcode for MOVDQU (load from memory to XMM)
    
    uint8_t rex = 0x40;  // Base REX prefix
    if (reg >= 8) {
        rex |= 0x4; // Set REX.R bit for XMM8-XMM15
        reg -= 8;
    }
    
    if (rex != 0x40) {
        instructions.push_back(rex);
    }
    
    // MOVDQU instruction to load XMM register from [rsp]
    instructions.push_back(movdqu_prefix);
    instructions.push_back(opcode_prefix);
    instructions.push_back(movdqu_load_opcode);
    
    int modrm = 0x04 | (reg << 3);  // ModRM byte: reg as destination, [rsp] as source
    int sib = 0x24;  // SIB byte for [rsp]
    
    instructions.push_back(modrm);
    instructions.push_back(sib);

    int rex_w = 0x48;        // REX.W prefix for 64-bit operation
    int add_opcode = 0x83;   // Opcode for ADD instruction
    int add_modrm = 0xc4;    // ModRM byte: ADD rsp, imm8
    int add_imm = 0x10;      // Immediate value 16 (bytes to add to rsp)
    
    // Add 16 bytes back to rsp to restore the stack
    instructions.push_back(rex_w);
    instructions.push_back(add_opcode);
    instructions.push_back(add_modrm);
    instructions.push_back(add_imm);
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

    free(code);

    return (compiled) mem;
}
