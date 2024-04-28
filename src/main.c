#include <stdio.h>
#include "hoard.h"

int main(int argc, char **argv)
{
    void* data;

    data = hmalloc(800);
    printf("data: %p\n", data);

    data = hmalloc(800);
    printf("data: %p\n", data);

    data = hmalloc(800);
    printf("data: %p\n", data);

    data = hmalloc(800);
    printf("data: %p\n", data);

    data = hmalloc(800);
    printf("data: %p\n", data);

    data = hmalloc(800);
    printf("data: %p\n", data);

    debug_print_hoard();


    return 0;
}