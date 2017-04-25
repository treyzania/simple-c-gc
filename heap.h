#pragma once
#include <stddef.h>
#include <stdlib.h>

typedef uint64_t heapsize;

typedef struct {
	heapsize size;
	void* body;
} heap;

heap* create_heap(heapsize size);
void free_heap(heap* h);
