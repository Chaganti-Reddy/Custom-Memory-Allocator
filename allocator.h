#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void init_allocator();
void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);
void* my_calloc(size_t num, size_t size);
void print_heap_layout();
void print_memory_stats();
void set_allocation_strategy(int strategy); // 0: First Fit, 1: Best Fit

#endif // ALLOCATOR_H
