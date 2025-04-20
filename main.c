#include <stdio.h>
#include "allocator.h"

void run_tests() {
    void* a = my_malloc(100);
    void* b = my_calloc(5, 20);
    void* c = my_malloc(200);
    print_heap_layout();
    print_memory_stats();

    my_free(b);
    my_free(a);
    print_heap_layout();
    print_memory_stats();

    void* d = my_malloc(180);
    void* e = my_realloc(c, 250);
    print_heap_layout();
    print_memory_stats();

    my_free(d);
    my_free(e);
    print_heap_layout();
    print_memory_stats();
}

int main() {
    init_allocator();

    printf("Custom Memory Allocator\n");
    printf("Choose allocation strategy:\n1. First Fit (default)\n2. Best Fit\n> ");
    int choice;
    scanf("%d", &choice);
    set_allocation_strategy((choice == 2) ? 1 : 0);

    run_tests();
    return 0;
}
