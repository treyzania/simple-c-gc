#include "heap.h"
#include <stdlib.h>

heap* heap_create(uint64_t size) {

	void* b = malloc(size);
	heap* h = malloc(sizeof(heap));
	h->size = size;
	h->body = b;
	return h;

}

void heap_free(heap* h) {
	free(h->body);
	free(h);
}
