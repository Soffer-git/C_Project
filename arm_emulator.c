#include "arm_emulator.h"

void arm_emulator_load_elf(arm_emulator_t* emulator, parsed_elf_t* elf) {
    memset(emulator, 0, sizeof(arm_emulator_t));

    for (int i = 0; i < elf->num_of_sections; i++)
    {
        memory_allocator_allocate_memory(&(emulator->memory), elf->sections[i].addr, elf->sections[i].size);
    }
    for (int i = 0; i < elf->num_of_program_sections; i++)
    {
        memory_allocator_allocate_memory(&(emulator->memory), elf->program_sections[i].addr, elf->program_sections[i].size);
    }
}