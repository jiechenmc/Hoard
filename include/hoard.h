#include <sys/types.h>
#define PRINT_SHORT 0
#define PRINT_VERBOSE 1

// void debug_print_hoard(int mode);

// int init_global_heap(void);

/*
 * Allocates 'size' bytes to the current thread.
 */
void *malloc(size_t size);

void free(void *ptr);