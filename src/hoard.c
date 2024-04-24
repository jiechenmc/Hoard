#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include "hoard.h"

#define HEAP_LIST_SIZE 10
#define PAGE_SIZE 4096
#define SUPERBLOCK_SIZE (PAGE_SIZE * 4)

#define GLOBAL_HEAP_SUCCESS 0
#define GLOBAL_HEAP_FAILURE -1
#define GLOBAL_HEAP_ALREADY_EXISTS -2


// definition of one Superblock. Acts as a singly-linked list.
typedef struct Superblock {
    Superblock* next;           // next superblock
    void* block;                // contents of this block
}
Superblock;

// definition of one thread map entry
typedef struct HeapListEntry {
    int tid;
    Superblock heap;
}
HeapListEntry;

// hashmap for threads. index 0 = global heap, has tid = -1
HeapListEntry heap_list[HEAP_LIST_SIZE] = {0};


// add global heap to thread list.
// return 0 on success or if already, -1 on failure.
int initialize_global_heap(void) {

    // check if already initialized
    if(heap_list[0].tid != 0)
        return GLOBAL_HEAP_ALREADY_EXISTS;
    
    // request superblock from virtual memory
    void* superblock = mmap(
        NULL,
        SUPERBLOCK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0);
    
    if(superblock == -1)
        return GLOBAL_HEAP_FAILURE;

    // if mmap succeeded
    heap_list[0].tid = -1;
    heap_list[0].heap = (Superblock){.block = superblock, .next = NULL};

    return GLOBAL_HEAP_SUCCESS;
}


// if thread id is in thread_map, return 0. Otherwise, return -1.
bool is_thread_in_map(void) {
    pid_t thread_id = gettid();

    // TODO: Change implementation to use hash function
    for(int i = 1; i < HEAP_LIST_SIZE; ++i)
    {
        if(heap_list[i].tid == thread_id)
            return true;
    }

    return false;
}

// TODO
int register_current_thread(void) {
    
    // TODO: Change implementation to match hashmap
    for(int i = 1; i < HEAP_LIST_SIZE; ++i)
    {
        if(heap_list[i].tid == 0)
        {
            heap_list[i].tid = gettid();
        }
    }

    return -1;
}

// TODO
int hmalloc(size_t size) {
    
    if(initialize_global_heap() == GLOBAL_HEAP_FAILURE)
        return -1;

    if(size == 0) return 0;

    /* if thread is already registered, attempt to allocate bytes in thread's
     * superblock. Else, create new superblock and allocate in that.
    */
    
    // register thread and give superblock if entry does not exist in threadmap
    if(is_thread_in_map() == false)
        register_current_thread();

    

    return 0;
}