#include "arm_emulator.h"
#include "arm_opcodes.h"
#include <string.h>

uint64_t sign_extend(uint64_t value, uint16_t bits)
{
    uint64_t sign = (1 << (bits - 1)) & value;
    uint64_t mask = ((~0UL) >> (bits - 1)) << (bits - 1);
    if (sign != 0)
        value |= mask;
    else
        value &= ~mask;
    return value;
}

void arm_emulator_STP_preindex(arm_emulator_t* emulator, uint32_t opcode)
{
    uint8_t Rt   = (opcode & 0x1f)     >> 0 ;
    uint8_t Rn   = (opcode & 0x3e0)    >> 5 ;
    uint8_t Rt2  = (opcode & 0x7c00)   >> 10;
    int8_t imm7  = (opcode & 0x3f8000) >> 15;
    printf("stp x%d, x%d, [x%d, #%lx]!\n", Rt, Rt2, Rn, (int64_t)sign_extend(imm7 * 8, 10));

    // TODO: Sign extend imm7
    int16_t n = (int16_t)sign_extend(imm7*8, 10);
    uint64_t addr = emulator->registers.X[Rn] + n;
    memory_allocation_t* page = memory_allocator_find_memory_record(&(emulator->memory), addr);
    *(uint64_t*)(&page->data[addr - page->addr]) = emulator->registers.X[Rt];
    *(uint64_t*)(&page->data[addr - page->addr + 8]) = emulator->registers.X[Rt2];
    emulator->registers.X[Rn] = addr;
    emulator->registers.PC += 4;
}

void arm_emulator_ADD(arm_emulator_t* emulator, uint32_t opcode)
{
    uint8_t Rt   = (opcode & 0x1f)     >> 0 ;
    uint8_t Rn   = (opcode & 0x3e0)    >> 5 ;
    int16_t imm12  = (opcode & 0x3ffc00)   >> 10;
    if (imm12 == 0) {
        printf("mov x%d, x%d\n", Rt, Rn);
    }
    else {
        printf("add x%d, [x%d, #%x]\n", Rt, Rn, imm12);
    }

    emulator->registers.X[Rt] = emulator->registers.X[Rn] + imm12;
    emulator->registers.PC += 4;

}

void arm_emulator_STR_unsigned_offset(arm_emulator_t* emulator, uint32_t opcode)
{
    uint8_t Rt   = (opcode & 0x1f)     >> 0 ;
    uint8_t Rn   = (opcode & 0x3e0)    >> 5 ;
    int16_t imm12  = (opcode & 0x3ffc00)   >> 10;
    printf("str x%d, [x%d, #0x%x]\n", Rt, Rn, imm12 * 8);

    uint64_t addr = emulator->registers.X[Rn] + imm12 * 8;
    memory_allocation_t* page = memory_allocator_find_memory_record(&(emulator->memory), addr);
    *(uint64_t*)(&page->data[addr - page->addr]) = emulator->registers.X[Rt];
    emulator->registers.X[Rn] = addr;
    emulator->registers.PC += 4;
}

void arm_emulator_STR_unsigned_offset_64(arm_emulator_t* emulator, uint32_t opcode)
{
    uint8_t Rt   = (opcode & 0x1f)     >> 0 ;
    uint8_t Rn   = (opcode & 0x3e0)    >> 5 ;
    int16_t imm12  = (opcode & 0x3ffc00)   >> 10;
    printf("str x%d, [x%d, #0x%x]\n", Rt, Rn, imm12 * 8);

    uint64_t addr = emulator->registers.X[Rn] + imm12 * 8;
    memory_allocation_t* page = memory_allocator_find_memory_record(&(emulator->memory), addr);
    *(uint64_t*)(&page->data[addr - page->addr]) = emulator->registers.X[Rt];
    emulator->registers.X[Rn] = addr;
    emulator->registers.PC += 4;
}

// Written as STR_64 - needs to be changed!
void arm_emulator_STR_unsigned_offset_32(arm_emulator_t* emulator, uint32_t opcode)
{
    uint8_t Rt   = (opcode & 0x1f)     >> 0 ;
    uint8_t Rn   = (opcode & 0x3e0)    >> 5 ;
    int16_t imm12  = (opcode & 0x3ffc00)   >> 10;
    printf("str w%d, [x%d, #0w%x]\n", Rt, Rn, imm12 * 4);

    uint64_t addr = emulator->registers.X[Rn] + imm12 * 4;
    memory_allocation_t* page = memory_allocator_find_memory_record(&(emulator->memory), addr);
    *(uint64_t*)(&page->data[addr - page->addr]) = emulator->registers.X[Rt];
    emulator->registers.X[Rn] = addr;
    emulator->registers.PC += 4;
}

void arm_emulator_MOVZ_64(arm_emulator_t* emulator, uint32_t opcode)
{
    uint8_t Rd    = (opcode & 0x1f)    >> 0;
    int16_t imm16 = (opcode & 0x1fffe) >> 5;
    uint8_t hw    = (opcode & 0x60000) >> 21;
    
    if (!(imm16 == 0 && hw != 0)) {
        printf("mov x%d, #%x\n", Rd, imm16);
    }
    else {
        printf("movz x%d, #%x LSL %d\n", Rd, imm16, hw*16);
    }
    
    emulator->registers.X[Rd] = imm16 << (hw * 16);
    emulator->registers.PC += 4;


}

void arm_emulator_MOVZ_32(arm_emulator_t* emulator, uint32_t opcode)
{
    uint8_t Rd    = (opcode & 0x1f)    >> 0;
    int16_t imm16 = (opcode & 0x1fffe) >> 5;
    uint8_t hw    = (opcode & 0x60000) >> 21;

    if (!(imm16 == 0 && hw != 0)) {
        printf("mov w%d, #%x\n", Rd, imm16);
    }
    else {
        printf("movz w%d, #%x LSL %d\n", Rd, imm16, hw*16);
    }

    emulator->registers.X[Rd] &= ~(uint64_t)0xffffffff;
    emulator->registers.X[Rd] |= imm16 << (hw * 16);
    emulator->registers.PC += 4;
}

void arm_emulator_BL(arm_emulator_t* emulator, uint32_t opcode)
{
    int32_t imm26 = sign_extend(opcode & 0x3ffffff, 26) * 4;
    printf("bl #0x%x\n", imm26);

    emulator->registers.X[30] = emulator->registers.PC;
    emulator->registers.PC = emulator->registers.PC + imm26;
    printf("new pc: %lx\n", emulator->registers.PC);
}


void opcode_tree_init(opcode_tree_t *tree)
{
    memset(tree, 0, sizeof(opcode_tree_t));
    opcode_tree_add_opcode(tree, "STP",  0b1010100110, 10, arm_emulator_STP_preindex);
    opcode_tree_add_opcode(tree, "ADD",  0b10010001, 8, arm_emulator_ADD);
    opcode_tree_add_opcode(tree, "STR",  0b1111100100, 10, arm_emulator_STR_unsigned_offset_64);
    opcode_tree_add_opcode(tree, "STR",  0b1011100100, 10, arm_emulator_STR_unsigned_offset_32);
    opcode_tree_add_opcode(tree, "MOVZ", 0b110100101, 9, arm_emulator_MOVZ_64);
    opcode_tree_add_opcode(tree, "MOVZ", 0b010100101, 9, arm_emulator_MOVZ_32);
    opcode_tree_add_opcode(tree, "BL",   0b100101, 6, arm_emulator_BL);
}