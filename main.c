#include <stdio.h>
#include "allocator.h"

int main(void)
{
    allocator_init();

    int *a = my_malloc(sizeof(int));
    int *b = my_malloc(sizeof(int));
    int *c = my_malloc(sizeof(int));

    if (a == NULL || b == NULL || c == NULL)
        return 1;

    *a = 100;
    *b = 200;
    *c = 300;

    printf("Before free:\n");
    printf("a = %p\n", (void *)a);
    printf("b = %p\n", (void *)b);
    printf("c = %p\n", (void *)c);

    // Release block B
    my_free(b);
    b = NULL;

    // Allocate another block
    int *d = my_malloc(sizeof(int));

    if (d == NULL)
        return 1;

    *d = 400;

    printf("\nAfter free:\n");
    printf("d = %p\n", (void *)d);
    printf("d value = %d\n", *d);

    return 0;
}