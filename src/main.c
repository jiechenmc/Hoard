#include <stdio.h>
#include <pthread.h>
#include "hoard.h"

void* thread_func(void* vargp)
{
    void* data = hmalloc(800);
    hfree(data);
}

int main(int argc, char **argv)
{
    for(int i = 0; i < 30; ++i)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, thread_func, NULL);
        pthread_join(tid, NULL);
    }
    debug_print_hoard(PRINT_VERBOSE);

    return 0;
}