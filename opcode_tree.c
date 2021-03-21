#include "opcode_tree.h"
#include <string.h>

void opcode_tree_add_opcode(opcode_tree_t *tree, const char* name, uint32_t opcode, int lengthInBits, opcode_handler_t handler)
{
    if (!tree->root)
    {
        tree->root = malloc(sizeof(node_t));
        memset(tree->root, 0, sizeof(node_t));
    }
    node_t *curr_node = tree->root;

    for (int i = 0; i < lengthInBits; i++)
    {
        node_t **next_node = (((opcode >> (lengthInBits - 1 - i)) & 0x1)) ? &curr_node->one : &curr_node->zero;
        if (!*next_node)
        {
            *next_node = malloc(sizeof(node_t));
            memset(*next_node, 0, sizeof(node_t));
        }
        curr_node = *next_node;
    }

    curr_node->name = name;
    curr_node->handler = *handler;

    if (curr_node->one || curr_node->zero)
    {
        printf("Warning! opcode collision detected, opcode: %s\n", name);
    }
}


opcode_handler_t opcode_tree_find_opcode(opcode_tree_t *tree, uint32_t opcode)
{
    node_t *curr_node = tree->root;
    if (!curr_node)
    {
        printf("No opcode handlers available.\n");
        return NULL;
    }

    int current_bit_index = 31;
    while (current_bit_index >= 0)
    {
        curr_node = (((opcode >> current_bit_index) & 0x1)) ? curr_node->one : curr_node->zero;
        if (!curr_node)
        {
            printf("Warning! unknown opcode detected, opcode: %x\n", opcode);
            return NULL;
        }

        if (curr_node->handler) {
            // printf("Found opcode: %s\n", curr_node->name);
            return curr_node->handler;
        }

        current_bit_index--;
    }
    printf("Warning! No handler found, opcode: %x\n", opcode);
    return NULL;
}