#include <stdint.h>
#include <sys/types.h>

typedef struct memory_allocation {
    size_t size;
    uint8_t* data;
    off_t addr;
    struct memory_allocation* next;
} memory_allocation_t;

typedef struct {
    memory_allocation_t* head;
    size_t amount;
} memory_table_t;

memory_allocation_t* memory_allocator_allocate_memory(memory_table_t* mem, off_t address, size_t size);
memory_allocation_t* memory_allocator_find_memory_record(memory_table_t* mem, off_t addr);
