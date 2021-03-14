#pragma once

#include "memory_allocator.h"
#include "elf_parser.h"

typedef struct 
{
    uint64_t X[32];
    uint64_t PC;
    uint64_t CPSR;
} arm_emulator_registers_t;


typedef struct {
    memory_table_t memory;
    arm_emulator_registers_t registers;
} arm_emulator_t;

void arm_emulator_load_elf(arm_emulator_t* emulator, parsed_elf_t* elf);

void arm_emulator_run(arm_emulator_t* emulator);