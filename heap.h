#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <glib.h>

typedef struct {
	uint64_t size;
	GArray* allocs;
	void* body;
} heap;

heap* heap_create(uint64_t size);
void heap_free(heap* h);
