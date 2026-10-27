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

static void split_block(BlockHeader *block, size_t required_size)
{
    const size_t header_size = align_size(sizeof(BlockHeader));
    const size_t min_payload = align_size(1);

    if (block->size < required_size)
        return;

    size_t remaining = block->size - required_size;

    if (remaining < header_size + min_payload)
        return;

    BlockHeader *new_block =
        (BlockHeader *)(void *)
        ((unsigned char *)block + header_size + required_size);

    new_block->size = remaining - header_size;
    new_block->is_free = 1;
    new_block->next = block->next;

    block->size = required_size;
    block->next = new_block;

    if (tail == block)
        tail = new_block;
}

static void merge_next(BlockHeader *block)
{
    const size_t header_size = align_size(sizeof(BlockHeader));

    while (block->next != NULL)
    {
        BlockHeader *next = block->next;

        unsigned char *expected = (unsigned char *)block + header_size + block->size;

        if (block->is_free == 0 ||
            next->is_free == 0 ||
            expected != (unsigned char *)next)
        {
            break;
        }

        block->size += header_size + next->size;
        block->next = next->next;

        if (tail == next)
            tail = block;
    }
}

// Merge all neighboring free blocks
static void coalesce_all(void)
{
    BlockHeader *current = head;

    while (current != NULL)
    {
        if (current->is_free)
            merge_next(current);

        current = current->next;
    }
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
            split_block(current, aligned_size);
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
            if (current->is_free)
                return;

            current->is_free = 1;
            coalesce_all();
            return;
        }

        current = current->next;
    }
}