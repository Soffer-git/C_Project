#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    uint64_t addr;
    char* name;
    size_t size;
    uint8_t* data;
} elf_section_t;

typedef struct {
    uint64_t addr;
    size_t size;
    uint8_t* data;
} elf_program_section_t;

typedef struct {
    uint64_t entry;
    size_t num_of_sections;
    elf_section_t* sections;
    
    size_t num_of_program_sections;
    elf_program_section_t* program_sections;
} parsed_elf_t;


void elf_parser_parse(FILE* file, parsed_elf_t* o_parsed_elf);
void elf_parser_free(parsed_elf_t* parsed_elf);