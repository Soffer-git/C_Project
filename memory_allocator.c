#include "memory_allocator.h"
#include <stdlib.h>

memory_allocation_t* memory_allocator_allocate_memory(memory_table_t* mem, off_t address, size_t size) {
    memory_allocation_t* allocation = malloc(sizeof(memory_allocation_t));
    allocation->data = malloc(size);
    allocation->addr = address;
    allocation->size = size;
    allocation->next = mem->head;
    mem->head = allocation;
    return allocation;
}

memory_allocation_t* memory_allocator_find_memory_record(memory_table_t* mem, off_t addr) {
    memory_allocation_t* curr = mem->head;
    while (curr != NULL)
    {
        if (addr >= curr->addr && addr < curr->addr + curr->size) return curr;
        curr = curr->next;
    }
    return NULL;
}