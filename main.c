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

    // Memory allocation confirmed
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

    // Stage 4 Test
    void *zero = my_malloc(0);
    assert(zero == NULL);

    my_free(NULL);

    printf("Stage 4 passed!\n");
    // Double-Free Detection
    void *g = my_malloc(32);

    assert(g != NULL);

    my_free(g);
    my_free(g);  // Double Free

    printf("\nDouble-Free test passed!\n");


    // Test 4: Invalid Pointer Detection
    int invalid_data = 123;
    my_free(&invalid_data);  // Invalid Pointer
    assert(invalid_data == 123);
    printf("Invalid Pointer test passed!\n");

    // Test 5: Interior Pointer Detection
    void *h = my_malloc(32);
    assert(h != NULL);
    my_free((unsigned char *)h + 1);
    printf("Interior Pointer test passed!\n");
    my_free(h);

    // Test 6: Oversized Allocation
    void *large = my_malloc(2048);
    assert(large == NULL);
    printf("Oversized Allocation test passed!\n");

    // Test 7: Integer Overflow
    void *overflow = my_malloc(SIZE_MAX);
    assert(overflow == NULL);
    printf("Integer Overflow test passed!\n");

    return 0;
}