#include <stdio.h>
#include "elf_parser.h"

int test();

int main(int argc, char* argv[]) {
    if (!test()) {
        printf("All tests passed!\n");
    } 
    else {
        printf("Tests failed. :(\n");
    }
}

int test() {
    FILE* file = fopen("emulate_me_arm64.out", "rb");
    parsed_elf_t elf;

    elf_parser_parse(file, &elf);
    printf("Entry is %x\n", elf.entry);

    if (elf.entry != 0x4001d8) {
        printf("entry is wrong.\n");
        return -1;
    }

    for (int i = 0; i < elf.num_of_program_sections; i++) {
        printf("Program section address 0x%x: ", elf.program_sections[i].addr);
        for (int j = 0; j < 3; j++)
        {
            printf("%x ", elf.program_sections[i].data[j]);
        }
    }
    return 0;
}