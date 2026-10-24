#include <stdio.h>
#include "allocator.h"

int main(void)
{
    allocator_init();

    int *a = my_malloc(sizeof(int));
    int *b = my_malloc(sizeof(int));
    int *c = my_malloc(sizeof(int));

    if (a == NULL || b == NULL || c == NULL)
    {
        printf("Allocation failed\n");
        return 1;
    }

    *a = 1;
    *b = 2233;
    *c = 445566;

    printf("a = %d\n", *a);
    printf("b = %d\n", *b);
    printf("c = %d\n", *c);

    printf("Address a = %p\n", (void *)a);
    printf("Address b = %p\n", (void *)b);
    printf("Address c = %p\n", (void *)c);

    return 0;
}