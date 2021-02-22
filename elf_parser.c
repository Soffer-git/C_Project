#include <stdio.h>
#include "elf_parser.h"

typedef struct {
    struct {
        uint8_t  ei_mag[4];     // 0x00 - 0x03
        uint8_t  ei_class;      // 0x04
        uint8_t  ei_data;       // 0x05
        uint8_t  ei_version;    // 0x06
        uint8_t  ei_osabi;      // 0x07
        uint8_t  ei_abiversion; // 0x08
        uint8_t  ei_pad[7];     // 0x09 - 0x0F
    } e_ident;
    uint16_t e_type;            // 0x10 - 0x11
    uint16_t e_machine;         // 0x12 - 0x13
    uint32_t e_version;         // 0x14 - 0x17
    uint64_t e_entry;           // 0x18 - 0x1F
    uint64_t e_phoff;           // 0x20 - 0x27
    uint64_t e_shoff;           // 0x28 - 0x2F
    uint32_t e_flags;           // 0x30 - 0x33
    uint16_t e_ehsize;          // 0x34 - 0x35 
    uint16_t e_phentsize;       // 0x36 - 0x37
    uint16_t e_phnum;           // 0x38 - 0x39
    uint16_t e_shentsize;       // 0x3A - 0x3B
    uint16_t e_shnum;           // 0x3C - 0x3D
    uint16_t e_shstrndx;        // 0x3E - 0x3F
} elf_header_t;

typedef struct {
    uint32_t p_type;            // 0x00 - 0x03
    uint32_t p_flags;           // 0x04 - 0x07
    uint64_t p_offset;          // 0x08 - 0x0F
    uint64_t p_vaddr;           // 0x10 - 0x17
    uint64_t p_paddr;           // 0x18 - 0x1F
    uint64_t p_filesz;          // 0x20 - 0x27
    uint64_t p_memsz;           // 0x28 - 0x2F
    uint64_t p_align;           // 0x30 - 0x37
} program_header_t;

typedef struct {
    uint32_t sh_name;           // 0x00 - 0x03
    uint32_t sh_type;           // 0x04 - 0x07
    uint64_t sh_flags;          // 0x08 - 0x0F
    uint64_t sh_addr;           // 0x10 - 0x17
    uint64_t sh_offset;         // 0x18 - 0x1F
    uint64_t sh_size;           // 0x20 - 0x27
    uint32_t sh_link;           // 0x28 - 0x2B
    uint32_t sh_info;           // 0x2C - 0x2F
    uint64_t sh_addralign;      // 0x30 - 0x37
    uint64_t sh_entsize;        // 0x38 - 0x3F
} section_header_t;

void get_from_file(FILE* file, uint8_t* buff, size_t size, long offset)
{
    long prev = ftell(file);
    fseek(file, offset, SEEK_SET);
    fread(buff, size, 1, file);
    fseek(file, prev, SEEK_SET);
}

void elf_parser_parse(FILE* file, parsed_elf_t* o_parsed_elf) {
    elf_header_t elf_header;
    fread(&elf_header, sizeof(elf_header), 1, file);
    o_parsed_elf->entry = elf_header.e_entry;
    o_parsed_elf->num_of_program_sections = elf_header.e_phnum;
    o_parsed_elf->num_of_sections = elf_header.e_shnum;

    o_parsed_elf->program_sections = malloc(o_parsed_elf->num_of_program_sections * sizeof(elf_program_section_t));
    o_parsed_elf->sections = malloc(o_parsed_elf->num_of_sections * sizeof(elf_section_t));
    
    fseek(file, elf_header.e_phoff, SEEK_SET);
    for (int i = 0; i < o_parsed_elf->num_of_program_sections; i++)
    {
        program_header_t program_header;
        fread(&program_header, sizeof(program_header), 1, file);
        if (program_header.p_type == NULL) continue;
        o_parsed_elf->program_sections[i].addr = program_header.p_vaddr;
        o_parsed_elf->program_sections[i].size = program_header.p_filesz;
        uint8_t* data_point = malloc(o_parsed_elf->program_sections[i].size);
        o_parsed_elf->program_sections[i].data = data_point;
        get_from_file(file, o_parsed_elf->program_sections[i].data, program_header.p_filesz, program_header.p_offset);
    }
    
    fseek(file, elf_header.e_shoff, SEEK_SET);
    for (int i = 0; i < o_parsed_elf->num_of_sections; i++)
    {
        section_header_t section_header;
        fread(&section_header, sizeof(section_header), 1, file);
        if (section_header.sh_type == NULL) continue;

        o_parsed_elf->sections[i].addr = section_header.sh_addr;
        o_parsed_elf->sections[i].size = section_header.sh_size;
        uint8_t* data_point = malloc(o_parsed_elf->sections[i].size);
        o_parsed_elf->sections[i].data = data_point;
        get_from_file(file, o_parsed_elf->sections[i].data, section_header.sh_size, section_header.sh_offset);
        
        printf("Section number %d: ", i);
        for (int j = 0; j < 3; j++)
        {
            printf("%x ", o_parsed_elf->sections[i].data[j]);
        }

        printf("\n\n");
    }

    fseek(file, elf_header.e_shoff, SEEK_SET);
    for (int i = 0; i < o_parsed_elf->num_of_sections; i++)
    {
        section_header_t section_header;
        fread(&section_header, sizeof(section_header), 1, file);
        o_parsed_elf->sections[i].name = &o_parsed_elf->sections[elf_header.e_shstrndx].data[section_header.sh_name];
        printf("Name number %d: %s ", i, o_parsed_elf->sections[i].name);

        printf("\n\n");
    }
}

void elf_parser_free(parsed_elf_t* parsed_elf) {

}