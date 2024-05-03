#include <stdio.h>
#include "hoard.h"

int main(int argc, char **argv)
{
    void* data1 = hmalloc(30);
    void* data2 = hmalloc(30);
    void* data3 = hmalloc(30);
    
    hfree(data2);

    debug_print_hoard();

    printf("data1: %p\n", data1);
    printf("data3: %p\n", data3);

    return 0;
}