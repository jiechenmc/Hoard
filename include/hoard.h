#include <sys/types.h>

void debug_print_hoard(void);

/*
 * Allocates 'size' bytes to the current thread. 
*/
void *hmalloc(size_t size);