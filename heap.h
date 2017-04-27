#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	size_t size;
	void* body;
} heap;

heap* heap_create(size_t size);
void heap_destroy(heap* h);

void* heap_malloc(heap* h, size_t s);
void heap_free(heap* h, void* p);
