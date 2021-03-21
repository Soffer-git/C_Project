#pragma once

#include <stdint.h>
#include "arm_emulator.h"

typedef void (*opcode_handler_t)(arm_emulator_t*, uint32_t);

typedef struct node
{
    struct node* zero;
    struct node* one;

    char const* name;
    opcode_handler_t handler;
} node_t;


typedef struct
{
    node_t* root;
} opcode_tree_t;


void opcode_tree_init(opcode_tree_t* tree);
void opcode_tree_add_opcode(opcode_tree_t *tree, const char* name, uint32_t opcode, int lengthInBits, opcode_handler_t handler);
opcode_handler_t opcode_tree_find_opcode(opcode_tree_t* tree, uint32_t opcode);
