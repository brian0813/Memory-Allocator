#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "allocator.h"

int main(void)
{
    allocator_init();

    // Initial allocation
    void *a = my_malloc(128);
    void *b = my_malloc(32);
    int *c = my_malloc(sizeof(int));

    assert(a != NULL && b != NULL && c != NULL);

    *c = 300;

    printf("Initial allocation:\n");
    printf("A = %p\n", a);
    printf("B = %p\n", b);
    printf("C = %p\n", (void *)c);

    // Test 1: Block Splitting
    my_free(a);

    void *d = my_malloc(32);
    void *e = my_malloc(64);

    assert(d != NULL && e != NULL);
    assert(d == a);

    printf("\nAfter splitting:\n");
    printf("D = %p\n", d);
    printf("E = %p\n", e);

    // Test 2: Block Coalescing
    my_free(d);
    my_free(e);
    my_free(b);

    // Requires merging free blocks
    void *f = my_malloc(176);

    assert(f != NULL);
    assert(f == a);
    assert(*c == 300);

    printf("\nAfter coalescing:\n");
    printf("F = %p\n", f);
    printf("C value = %d\n", *c);

    printf("\nAll tests passed!\n");

    return 0;
}