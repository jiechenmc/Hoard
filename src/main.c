#include <stdio.h>
#include <pthread.h>
#include "hoard.h"

int main(int argc, char **argv)
{
    hmalloc(800);
    debug_print_hoard(PRINT_VERBOSE);

    return 0;
}