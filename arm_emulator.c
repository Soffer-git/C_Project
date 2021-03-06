#include "arm_emulator.h"
#include <string.h>
#include <byteswap.h>

void arm_emulator_load_elf(arm_emulator_t* emulator, parsed_elf_t* elf) {
    memset(emulator, 0, sizeof(arm_emulator_t));

    for (int i = 0; i < elf->num_of_sections; i++)
    {
        memory_allocation_t* page = memory_allocator_allocate_memory(&(emulator->memory), elf->sections[i].addr, elf->sections[i].size);
        memcpy(page->data, elf->sections[i].data, elf->sections[i].size);
    }
    for (int i = 0; i < elf->num_of_program_sections; i++)
    {
        memory_allocation_t* page = memory_allocator_allocate_memory(&(emulator->memory), elf->program_sections[i].addr, elf->program_sections[i].size);
        memcpy(page->data, elf->program_sections[i].data, elf->program_sections[i].size);
    }

    emulator->registers.PC = elf->entry;
}

// const char *bit_rep[16] = {
//     [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
//     [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
//     [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
//     [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
// };

// void print_byte(uint8_t byte)
// {
//     printf("%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
// }

void arm_emulator_run(arm_emulator_t* emulator)
{
    while (1)
    {
        memory_allocation_t* page = memory_allocator_find_memory_record(&emulator->memory, emulator->registers.PC);
        uint32_t opcode = *(uint32_t*)(&page->data[emulator->registers.PC - page->addr]);
        switch ((opcode & (0x1ff << 21)) >> 22) {
            case 0b010100110: // STP
                printf("Found STP\n");
        }
        printf("PC = 0x%lx\n", emulator->registers.PC);
        return;
    }
}
