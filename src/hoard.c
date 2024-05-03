#define _GNU_SOURCE
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "hoard.h"

#define HEAP_LIST_SIZE 11
#define PAGE_SIZE 4096
#define SUPERBLOCK_SIZE (PAGE_SIZE * 1)
#define BLOCK_HEADER_SIZE 8
#define MIN_SIZE_CLASS 32

#define GLOBAL_HEAP_SUCCESS 0
#define GLOBAL_HEAP_FAILURE -1
#define GLOBAL_HEAP_ALREADY_EXISTS -2

#define BLOCK_MMAP 0x2
#define BLOCK_USED 0x1

typedef struct Block Block;
typedef struct SuperblockNode SuperblockNode;
typedef struct HeapListEntry HeapListEntry;

// definition of one block within a superblock
struct Block {
    long header;
    void *payload;
};

// definition of one Superblock. Acts as a singly-linked list.
struct SuperblockNode {
    int size_class;
    int num_free_blocks;
    char* data;

    // if allocated, points to next superblock in heap.
    // if in free list, points to next free superblock.
    SuperblockNode* next;
};

// definition of one thread map entry
struct HeapListEntry {
    pid_t tid;
    int m_use;
    int m_alloc;
    SuperblockNode* heap;
};

// hashmap for threads. index 0 = global heap, has tid = 0
HeapListEntry heap_list[HEAP_LIST_SIZE] = {0};

bool global_heap_initialized = false;

// linked list head for superblock nodes obtained via sbrk()
SuperblockNode* sb_node_list = NULL;


// prints out hoard info for all threads/heaps
void debug_print_hoard(void) {
    for(int i = 0; i < HEAP_LIST_SIZE; ++i)
    {
        if(heap_list[i].heap == NULL)
        {
            printf("========\n");
            break;
        }

        printf("========\n");
        printf("TID: %i\n", heap_list[i].tid);
        printf("In use: %i\n", heap_list[i].m_use);
        printf("Allocated: %i\n", heap_list[i].m_alloc);

        SuperblockNode* curr_node = heap_list[i].heap;

        while(curr_node != NULL)
        {
            printf("Superblock: %p    data: %p    size class: %i    free blocks: %i\n",
                curr_node,
                curr_node->data,
                curr_node->size_class,
                curr_node->num_free_blocks);

            // print block info
            if(curr_node->size_class > 0)
            {
                for(int i = 0; i < SUPERBLOCK_SIZE / curr_node->size_class; ++i)
                {
                    void* block_pointer = curr_node->data + i * curr_node->size_class;

                    // only print allocated blocks
                    long block_header = *(long*)(block_pointer);

                    if(block_header & BLOCK_USED)
                        printf("\tblock @ %p: 0x%lx\n", block_pointer, block_header);
                }
            }

            curr_node = curr_node->next;
        }
    }
}

// get superblock from virtual memory and wrap it in a superblock node
// returns -1 on failure
void *get_superblock(void) {

    SuperblockNode* sb_node = 0;

    void* data = mmap(
        NULL,
        SUPERBLOCK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0);

    // if mmap failed
    if(data == (void*) -1)
        return data;

    // if no free superblock nodes, get one from memory
    if(sb_node_list == NULL)
    {
        sb_node = sbrk(sizeof(SuperblockNode));

        // TODO: Handle sbrk failure
    }
    else // Get from free superblock list
    {
        sb_node = sb_node_list;
        sb_node_list = sb_node_list->next;
    }

    // set lower 3 bits of every 8 bytes to 0
    for(int i = 0; i < SUPERBLOCK_SIZE / 8; ++i)
        *((long*)data + i) &= ~0b111;

    sb_node->size_class = 0;
    sb_node->num_free_blocks = SUPERBLOCK_SIZE / MIN_SIZE_CLASS;
    sb_node->next = NULL;
    sb_node->data = data;

    return sb_node;
}


// add global heap to thread list.
// return 0 on success, -1 on failure, -2 if already exists.
int initialize_global_heap(void) {

    // check if already initialized
    if(global_heap_initialized)
        return GLOBAL_HEAP_ALREADY_EXISTS;
    
    SuperblockNode* sb_node = get_superblock();
    
    if(sb_node == (void*) -1)
        return GLOBAL_HEAP_FAILURE;

    heap_list[0].tid = 0;
    heap_list[0].m_use = 0;
    heap_list[0].m_alloc = SUPERBLOCK_SIZE;
    heap_list[0].heap = sb_node;

    global_heap_initialized = true;

    return GLOBAL_HEAP_SUCCESS;
}


// check if thread id is in heap_list
bool is_thread_registered(void) {
    pid_t tid = gettid();

    // TODO: Change implementation to use hash function
    for(int i = 1; i < HEAP_LIST_SIZE; ++i)
    {
        if(heap_list[i].tid == tid)
            return true;
    }

    return false;
}


// Adds current thread to heap_list. If successful, returns 0. Else, returns -1.
int register_current_thread(void) {
    
    // TODO: Change implementation to match hashmap
    for(int i = 1; i < HEAP_LIST_SIZE; ++i)
    {
        if(heap_list[i].tid == 0)
        {
            heap_list[i].tid = gettid();
            heap_list[i].m_use = 0;
            heap_list[i].m_alloc = 0;
            heap_list[i].heap = NULL;
            return 0;
        }
    }

    return -1;
}


// get heap belonging to this thread
// return heap entry pointer if found, NULL otherwise.
HeapListEntry* get_thread_heap(void) {
    pid_t tid = gettid();
    for(int i = 0; i < HEAP_LIST_SIZE; ++i)
    {
        if(heap_list[i].tid == tid)
        {
            return &heap_list[i];
        }
    }

    return NULL;
}


// get pointer to global heap
HeapListEntry* get_global_heap(void) {
    return &heap_list[0];
}


// TODO
void *hmalloc(size_t size) {

    if(initialize_global_heap() == GLOBAL_HEAP_FAILURE)
        return NULL;

    if(size == 0)
        return NULL;

    // If size is greater than S/2, allocate via mmap
    if(size + BLOCK_HEADER_SIZE > SUPERBLOCK_SIZE / 2)
    {
        Block* data = mmap(
            NULL,
            size + BLOCK_HEADER_SIZE,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1, 0);

        data->header = ((size + BLOCK_HEADER_SIZE) << 3) | BLOCK_MMAP | BLOCK_USED;

        return &(data->payload);
    }

    // Size is <= S/2
    if(!is_thread_registered())
        register_current_thread();

    // attempt to allocate in thread's superblock. Else, create new superblock
    HeapListEntry* heap_entry = get_thread_heap();
    if(!heap_entry->heap)
    {
        heap_entry->heap = get_superblock();
        heap_entry->m_alloc = SUPERBLOCK_SIZE;
    }

    size_t alloc_size = size + BLOCK_HEADER_SIZE;

    // iterate superblocks, find most full one with matching size class and sufficient space
    int least_free = SUPERBLOCK_SIZE;               // acts as infinity, since no superblock can have same number of blocks as its size in bytes
    SuperblockNode* least_free_node = NULL;
    SuperblockNode* curr_node = heap_entry->heap;

    // converts alloc_size to nearest size class
    // ex. 96 -> 128, minimum 32
    int size_class = pow(2, ceil(log2(alloc_size)));

    if(size_class < 32)
        size_class = 32;

    while(curr_node != NULL)
    {
        if(curr_node->size_class != 0 && curr_node->size_class != size_class)
        {
            curr_node = curr_node->next;
            continue;
        }

        // superblock is either empty or has matching size class
        if(curr_node->num_free_blocks > 0 && curr_node->num_free_blocks < least_free)
        {
            least_free = curr_node->num_free_blocks;
            least_free_node = curr_node;
        }

        curr_node = curr_node->next;
    }

    // no available superblock found
    if(least_free_node == NULL)
    {
        // TODO: Get new superblock from virtual memory and allocate in that
        least_free_node = get_superblock();
        least_free_node->next = heap_entry->heap;
        heap_entry->heap = least_free_node;
        heap_entry->m_alloc += SUPERBLOCK_SIZE;
    }

    // superblock found, set attributes in case it is 0
    if(least_free_node->size_class == 0)
    {
        least_free_node->size_class = size_class;
        least_free_node->num_free_blocks = SUPERBLOCK_SIZE / size_class;
    }
    

    // locate free block in least_free_node
    void* block_pointer = least_free_node->data;
    while(true)
    {
        long block_header = *((long*)block_pointer);

        if((block_header & BLOCK_USED) == 0)
            break;

        block_pointer += least_free_node->size_class;
    }

    // available block found
    *((long*)block_pointer) = (long)least_free_node | BLOCK_USED;

    // decrement free blocks
    least_free_node->num_free_blocks--;

    // update usage statistics
    heap_entry->m_use += size_class;

    return block_pointer + 8;
}

void hfree(void* ptr) {
    if(!ptr)
        return;

    long* header = ptr - 8;

    // clear allocated bit
    *header &= ~BLOCK_USED;

    // update superblock info
    SuperblockNode* sp = (void*)(*header & ~0b111);
    sp->num_free_blocks++;
    
    // update heap entry
    HeapListEntry* hentry = get_thread_heap();
    hentry->m_use -= sp->size_class;

    // if no more allocated blocks in superblock
    if(sp->num_free_blocks == SUPERBLOCK_SIZE / sp->size_class)
    {
        sp->size_class = 0;
        sp->num_free_blocks = SUPERBLOCK_SIZE / MIN_SIZE_CLASS;
    }
}