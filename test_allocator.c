#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "allocator.h"

// Test 1: Basic Allocation
static void test_basic_allocation(void)
{
    allocator_init();

    int *a = my_malloc(sizeof(int));
    assert(a != NULL);

    *a = 100;
    assert(*a == 100);

    my_free(a);
}

// Test 2: Block Reuse
static void test_block_reuse(void)
{
    allocator_init();

    void *a = my_malloc(64);
    assert(a != NULL);

    my_free(a);

    void *b = my_malloc(64);
    assert(b != NULL);
    assert(a == b);

    my_free(b);
}

// Test 3: Block Splitting
static void test_block_splitting(void)
{
    allocator_init();

    void *a = my_malloc(128);
    assert(a != NULL);

    my_free(a);

    void *b = my_malloc(32);
    void *c = my_malloc(32);

    assert(b != NULL);
    assert(c != NULL);
    assert(b == a);
    assert(b != c);

    my_free(b);
    my_free(c);
}

// Test 4: Block Coalescing
static void test_block_coalescing(void)
{
    allocator_init();

    void *a = my_malloc(128);
    void *b = my_malloc(32);
    int *c = my_malloc(sizeof(int));

    assert(a != NULL && b != NULL && c != NULL);
    *c = 300;

    my_free(a);

    void *d = my_malloc(32);
    void *e = my_malloc(64);

    assert(d != NULL && e != NULL);
    assert(d == a);

    my_free(d);
    my_free(e);
    my_free(b);

    void *f = my_malloc(176);

    assert(f != NULL);
    assert(f == a);
    assert(*c == 300);

    my_free(f);
    my_free(c);
}

// Test 5: Invalid Free & Double Free
static void test_invalid_free(void)
{
    allocator_init();

    void *a = my_malloc(64);
    assert(a != NULL);

    my_free(a);
    my_free(a);  // Double Free

    int invalid = 123;
    my_free(&invalid);

    assert(invalid == 123);

    void *b = my_malloc(32);
    assert(b != NULL);

    my_free((unsigned char *)b + 1);

    // Verify the interior pointer did not free b.
    void *c = my_malloc(32);
    assert(c != NULL);
    assert(c != b);

    my_free(b);
    my_free(c);
    my_free(NULL);
}

// Test 6: Allocation Boundary
static void test_allocation_boundary(void)
{
    allocator_init();

    assert(my_malloc(0) == NULL);
    assert(my_malloc(2048) == NULL);
    assert(my_malloc(SIZE_MAX) == NULL);

    void *a = my_malloc(32);
    assert(a != NULL);

    my_free(a);
}

// Run a test and print its result
#define RUN_TEST(test_func)                  \
    do {                                     \
        printf("[ RUN  ] %s\n", #test_func); \
        test_func();                         \
        printf("[ PASS ] %s\n", #test_func); \
    } while (0)

// Test 7: Stress Test
static void test_stress(void)
{
    allocator_init();

    for (int i = 0; i < 100000; i++)
    {
        void *p = my_malloc(32);

        assert(p != NULL);

        my_free(p);
    }
}

int main(void)
{
    printf("Memory Allocator Test Suite\n\n");

    RUN_TEST(test_basic_allocation);
    RUN_TEST(test_block_reuse);
    RUN_TEST(test_block_splitting);
    RUN_TEST(test_block_coalescing);
    RUN_TEST(test_invalid_free);
    RUN_TEST(test_allocation_boundary);
    RUN_TEST(test_stress);

    printf("\nAll tests passed!\n");

    return 0;
}