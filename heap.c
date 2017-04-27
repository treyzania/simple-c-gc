#include "utils.h"
#include "heap.h"
#include <glib.h>

typedef struct {

	GArray* allocs;
	heap the_heap;

} heapmeta;

typedef struct alloc_t {
	size_t size;
	void* body;
} alloc;

heap* heap_create(uint64_t size) {

	heapmeta* hm = malloc(sizeof(heapmeta));
	hm->allocs = g_array_new(FALSE, FALSE, sizeof(alloc*));

	// Return it.
	return &(hm->the_heap);

}

static heapmeta* _find_heapmeta(heap* h) {
	return (heapmeta*) (h - FIELD_OFFSET(heapmeta, the_heap));
}

void heap_destroy(heap* h) {

	heapmeta* hm = _find_heapmeta(h);
	free(h->body);
	free(hm);

}

void* heap_malloc(heap* h, size_t s) {

	heapmeta* hm = _find_heapmeta(h);
	alloc* a = malloc(sizeof(alloc*));
	a->size = s;
	if (s > h->size) return NULL;

	guint num = hm->allocs->len;
	void* end = h->body + h->size;
	if (num >= 2) {

		/*
		 * Now we have to do some scary things to see if we can fit it in
		 * between two other allocations, in case we free one larger than us
		 * before we try to add ourselves.
		 */
		void* last = NULL;
		for (guint i = 0; i < num - 1; i++) {

			alloc* here = g_array_index(hm->allocs, alloc*, i);
			alloc* next = g_array_index(hm->allocs, alloc*, i + 1);
			size_t diff = next->body - here->body - here->size;
			last = next->body + next->size;

			if (diff >= s) {

				a->body = here->body + here->size;
				if (a->body + a->size > end) {
					return NULL;
				} else {
					g_array_insert_val(hm->allocs, i, a);
					return a->body;
				}

			}

		}

		if (last + a->size < end) {
			a->body = last;
			g_array_append_val(hm->allocs, a);
			return a->body;
		} else {
			return NULL;
		}

	} else if (num == 1) {

		alloc* first = g_array_index(hm->allocs, alloc*, 0);

		// Since this is the first, we can just make it at the first spot.
		g_array_append_val(hm->allocs, a);
		a->body = first->body + first->size;
		if (a->body + a->size > end) {
			return NULL;
		} else {
			return a->body;
		}

	} else { // It must be 0.

		// Since this is the first, we can just make it at the first spot.
		g_array_append_val(hm->allocs, a);
		a->body = h;
		return a->body;

	}

}

void heap_free(heap* h, void* p) {

	heapmeta* hm = _find_heapmeta(h);
	for (guint i = 0; i < hm->allocs->len; i++) {

		alloc* a = g_array_index(hm->allocs, alloc*, i);
		if (a->body == p) {
			g_array_remove_index(hm->allocs, i);
			return;
		}

	}

}
