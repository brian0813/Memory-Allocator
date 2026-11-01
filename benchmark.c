#include <stdio.h>
#include <time.h>
#include "allocator.h"

int main(void)
{
    const int iterations = 1000000;

    allocator_init();

    clock_t start = clock();

    for (int i = 0; i < iterations; i++)
    {
        void *p = my_malloc(32);

        if (p == NULL)
        {
            printf("Allocation failed!\n");
            return 1;
        }

        my_free(p);
    }

    clock_t end = clock();

    double elapsed =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("Iterations: %d\n", iterations);
    printf("Total CPU time: %.6f seconds\n", elapsed);
    printf("Average time: %.3f ns\n",
           elapsed * 1e9 / iterations);

    return 0;
}