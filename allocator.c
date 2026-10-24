#include <stdint.h>
#include "allocator.h"
#include <stddef.h>

#define POOL_SIZE 1024

static union {
    max_align_t alignment;
    unsigned char bytes[POOL_SIZE]; // Declare the size of the array and establish a fixed memory space.
} memory_pool;

static size_t offset = 0;

void allocator_init(void)
{
    offset = 0;
}

void *my_malloc(size_t size)
{
    const size_t alignment = _Alignof(max_align_t);

    if (size == 0)
        return NULL;

    // If the size exceeds the safety limit, the memory allocation will be rejected to avoid overflow during subsequent addition.
    if (size > SIZE_MAX - (alignment - 1))
        return NULL;

    size_t aligned_size =
        ((size + alignment - 1) / alignment) * alignment;

    // Check if there is enough remaining memory.
    if (aligned_size > POOL_SIZE - offset)
        return NULL;

    void *ptr = &memory_pool.bytes[offset]; // Get the memory address of the element.

    offset += aligned_size;

    return ptr;
}