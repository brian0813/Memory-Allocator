#include <stdint.h>
#include "allocator.h"
#include <stddef.h>

#define POOL_SIZE 1024

typedef struct BlockHeader
{
    size_t size;
    int is_free;
    struct BlockHeader *next;
} BlockHeader;

static union {
    max_align_t alignment;
    unsigned char bytes[POOL_SIZE]; // Declare the size of the array and establish a fixed memory space.
} memory_pool;

static size_t offset = 0;
static BlockHeader *head = NULL;
static BlockHeader *tail = NULL;

static size_t align_size(size_t size)
{
    const size_t alignment = _Alignof(max_align_t);

    // If the size exceeds the safety limit, the memory allocation will be rejected to avoid overflow during subsequent addition.
    if (size > SIZE_MAX - (alignment - 1))
        return 0;

    return ((size + alignment - 1) / alignment) * alignment;
}

void allocator_init(void)
{
    offset = 0;
    head = NULL;
    tail = NULL;
}

void *my_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    size_t aligned_size = align_size(size);

    if (aligned_size == 0)
        return NULL;

    const size_t header_size = align_size(sizeof(BlockHeader));

    // Step 1: Search for a reusable block
    BlockHeader *current = head;

    while (current != NULL)
    {
        if (current->is_free && current->size >= aligned_size)
        {
            current->is_free = 0;

            return (unsigned char *)current + header_size;
        }

        current = current->next;
    }

    // Step 2: Create a new block if none is reusable
    if (header_size > POOL_SIZE - offset)
        return NULL;

    if (aligned_size > POOL_SIZE - offset - header_size)
        return NULL;

    // Point to a new block that is about to be created.
    BlockHeader *block =
        (BlockHeader *)(void *)&memory_pool.bytes[offset];

    block->size = aligned_size;
    block->is_free = 0;
    block->next = NULL;

    if (tail != NULL)
        tail->next = block;
    else
        head = block;

    tail = block;

    offset += header_size + aligned_size;

    return (unsigned char *)block + header_size;
}

void my_free(void *ptr)
{
    if (ptr == NULL)
        return;

    const size_t header_size = align_size(sizeof(BlockHeader));

    BlockHeader *current = head;

    while (current != NULL)
    {
        void *payload = (unsigned char *)current + header_size;

        if (payload == ptr)
        {
            current->is_free = 1;
            return;
        }

        current = current->next;
    }
}