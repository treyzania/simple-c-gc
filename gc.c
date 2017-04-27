#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "gc.h"
#include "obj.h"
#include "heap.h"
#include "utils.c"

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

oref create_object(otype* t, objheap* h) {

	void* o = heap_malloc(h->mem, t->size);
	memset(o, 0, t->size);

	// Set up the object header so that we have a reference to the type now.
	objheader* head = (objheader*) o;
	head->type = t;
	head->objectflags = 0;

	// Now to make a reference to it to return.
	oref ref;
	ref.heap = h->mem;
	ref.ptr = o - (void*) (h->mem);

	// Add it to the list of objects in the everything.
	allocref* a = malloc(sizeof(allocref));
	a->offset = ref.ptr;
	a->flags = OA_NORMAL;
	h->allocs = g_list_prepend(h->allocs, a);

	/*
	 * We prepend it up there so that short-lived objects end up getting removed
	 * faster than longer-lived objects, more or less.
	 */

	return ref;

}

void free_object(oref ref) {

	// Free the object on the heap.
	heap_free(ref.heap, (void*) ref.heap + ref.ptr);

	// Remove the allocation from the list of allocations.
	objheap* oh = (objheap*) (ref.heap - FIELD_OFFSET(objheap, mem));
	GList* l = oh->allocs;
	while (l != NULL) {

		GList* next = l->next;
		allocref* aref = (allocref*) l->data;
		if (aref->offset == ref.ptr) {
			oh->allocs = g_list_delete_link(oh->allocs, l);
			free(aref);
		}

	}

}

objheap* create_objheap(size_t hsize, otype* root) {

	objheap* oh = malloc(sizeof(objheap));
	heap* h = heap_create(hsize);
	oh->mem = h;
	oh->allocs = NULL;

	// Now set up the GC root object.
	oref ref = create_object(root, oh);
	oh->root_offset = ref.ptr;

	return oh;

}

void objheap_objheap(objheap* oh) {

	heap_destroy(oh->mem);
	g_list_free(oh->allocs);
	free(oh);

}

oref copy_object(oref ref, objheap* dest) {

	// We need to get the type multiple times anyways.
	otype* t = get_object_type(ref);

	// Create the "new" object, then copy the data.
	oref nr = create_object(t, dest);
	memcpy(resolve_object(nr), resolve_object(ref), t->size);

	// Then just return the ref to the new object.
	return nr;

}
