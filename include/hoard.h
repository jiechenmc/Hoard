#include <sys/types.h>

void debug_print_hoard(void);

int initialize_global_heap(void);

/*
 * Allocates 'size' bytes to the current thread. 
*/
void *hmalloc(size_t size);

void hfree(void* ptr);