#include "allocator.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HEAP_SIZE 1024 * 1024

static char heap[HEAP_SIZE];

typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

#define BLOCK_SIZE sizeof(Block)

static Block* free_list = NULL;
static int use_best_fit = 0;

void init_allocator() {
    free_list = (Block*)heap;
    free_list->size = HEAP_SIZE - BLOCK_SIZE;
    free_list->free = 1;
    free_list->next = NULL;
}

void set_allocation_strategy(int strategy) {
    use_best_fit = strategy;
}

static void split_block(Block* block, size_t size) {
    Block* new_block = (Block*)((char*)block + BLOCK_SIZE + size);
    new_block->size = block->size - size - BLOCK_SIZE;
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->free = 0;
    block->next = new_block;
}

static void merge_blocks() {
    Block* current = free_list;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

static Block* find_block(size_t size) {
    Block* current = free_list;
    Block* best_block = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (!use_best_fit) {
                return current; // First Fit returns immediately
            } else {
                if (!best_block || current->size < best_block->size) {
                    best_block = current;
                }
            }
        }
        current = current->next;
    }

    return use_best_fit ? best_block : NULL;
}

void* my_malloc(size_t size) {
    Block* block = find_block(size);
    if (!block) return NULL;

    printf("[ALLOC] Selected block at %p | Size: %zu\n", (void*)block, block->size);

    if (block->size >= size + BLOCK_SIZE + 8) {
        split_block(block, size);
    } else {
        block->free = 0;
    }
    return (void*)(block + 1);
}

void my_free(void* ptr) {
    if (!ptr) return;
    Block* block = (Block*)ptr - 1;
    block->free = 1;
    printf("[FREE] Freed block at %p | Size: %zu\n", (void*)block, block->size);
    merge_blocks();
}

void* my_realloc(void* ptr, size_t size) {
    if (!ptr) return my_malloc(size);
    Block* old_block = (Block*)ptr - 1;
    if (old_block->size >= size) return ptr;

    void* new_ptr = my_malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, old_block->size);
        my_free(ptr);
    }
    return new_ptr;
}

void* my_calloc(size_t num, size_t size) {
    size_t total_size = num * size;
    void* ptr = my_malloc(total_size);
    if (ptr) memset(ptr, 0, total_size);
    return ptr;
}

void print_heap_layout() {
    Block* current = free_list;
    int index = 0;
    printf("\n--- HEAP LAYOUT ---\n");
    while (current) {
        printf("Block %d | Addr: %p | Size: %zu | %s\n", index++, (void*)current, current->size, current->free ? "Free" : "Used");
        current = current->next;
    }
    printf("--------------------\n");
}

void print_memory_stats() {
    size_t used = 0, free = 0;
    Block* current = free_list;
    while (current) {
        if (current->free) free += current->size;
        else used += current->size;
        current = current->next;
    }
    printf("\n[MEMORY STATS] Used: %zu bytes | Free: %zu bytes\n", used, free);
}
