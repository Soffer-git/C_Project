#include "memory_allocator.h"
#include "elf_parser.h"

typedef struct {
    memory_table_t memory;
} arm_emulator_t;

void arm_emulator_load_elf(arm_emulator_t* emulator, parsed_elf_t* elf);