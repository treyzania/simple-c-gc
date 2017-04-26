#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	uint64_t size;
	void* body;
} heap;

heap* create_heap(uint64_t size);
void free_heap(heap* h);
