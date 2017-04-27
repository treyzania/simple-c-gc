#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "gc.h"
#include "obj.h"
#include "heap.h"

static uint64_t _count_references(oref ref, oref* roots, uint64_t root_cnt) {

	uint64_t cnt = 0;
	for (int i = 0; i < root_cnt; i++) {

		if (roots[i].heap != roots[0].heap) continue; // FIXME This won't count references into other heaps.

		otype* t = get_object_type(roots[i]);
		for (int j = 0; j < t->member_cnt; j++) {

			if (!IS_REFERENCE(roots[i], j)) continue;
			oref m = convert_to_oref((void*) (resolve_object(ref) + compute_offset(t, j)), ref.heap);

			if (OREF_EQUAL(ref, m)) {
				cnt++;
			} else {
				cnt += _count_references(ref, &m, 1); // Hacky hack.
			}

		}

	}

	return cnt;

}

oref create_object(otype* t, heap* h) {

	void* o = heap_malloc(h, t->size);
	memset(o, 0, t->size);

	// Set up the object header so that we have a reference to the type now.
	objheader* head = (objheader*) o;
	head->type = t;
	head->objectflags = 0;

	// Now to make a reference to it to return.
	oref ref;
	ref.heap = h;
	ref.ptr = o - (void*) h;
	return ref;

}

void free_object(oref ref) {
	heap_free(ref.heap, (void*) ref.heap + ref.ptr);
}
