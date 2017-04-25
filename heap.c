#include "heap.h"
#include <stdlib.h>

heap* create_heap(heapsize size) {

	void* b = malloc(size);
	heap* h = malloc(sizeof(heap));
	h->size = size;
	h->body = b;
	return h;

}

void free_heap(heap* h) {
	free(h->body);
	free(h);
}
