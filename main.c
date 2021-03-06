#include <stdio.h>
#include "elf_parser.h"
#include "arm_emulator.h"

int test();

/* Missions:
1. Some of the sections have an address of 0 - probably should be fixed. :D
2. Use the elf_parser_free() function.
3. Complete the STP handling.
4. The main while loop in arm_emulator_run should have a more meaningful parameter.
*/

int main(int argc, char* argv[]) {
    FILE* file = fopen("emulate_me_arm64.out", "rb");
    parsed_elf_t elf;
    arm_emulator_t emulator;

    elf_parser_parse(file, &elf);
    arm_emulator_load_elf(&emulator, &elf);
    arm_emulator_run(&emulator);
    test(); // Only prints information for now...
    return 0;
}

int test() {
    FILE* file = fopen("emulate_me_arm64.out", "rb");
    parsed_elf_t elf;

    elf_parser_parse(file, &elf);
    printf("Entry is %lx", elf.entry);
    if (elf.entry != 0x4001d8) return -1;

    for (int i = 0; i < elf.num_of_program_sections; i++) {
        printf("\n\nProgram section number %x\nAddress: 0x%lx\nSize: 0x%lx\nData: ", i, elf.program_sections[i].addr, elf.program_sections[i].size);
        for (int j = 0; j < 3; j++)
        {
            printf("%x", elf.program_sections[i].data[j]);
        }
    }

    for (int i = 0; i < elf.num_of_sections; i++) {
        printf("\n\nSection number %x\nAddress: 0x%lx\nExtension: %s\nSize: 0x%lx\nData: ", i, elf.sections[i].addr, elf.sections[i].name, elf.sections[i].size);
        for (int j = 0; j < 3; j++)
        {
            printf("%x", elf.sections[i].data[j]);
        }
    }
    return 0;
}