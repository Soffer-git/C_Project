#include <stdio.h>
#include "elf_parser.h"

int main(int argc, char* argv[]) {
    FILE* file = fopen("emulate_me_arm64.out", "rb");
    parsed_elf_t elf;

    elf_parser_parse(file, &elf);
}