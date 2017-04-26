#include "gc.h"

oref create_object(otype* t, heap* h) {

}

void free_object(oref ref) {

}

uint64_t __count_references(oref ref, oref* roots, uint64_t root_cnt) {

	// Check to see if all the refs are in the same heap.
	for (int i = 0; i < root_cnt; i++) {
		if (ref.heap != roots[i].heap) return -1; // If not then forget it.
	}

	uint64_t cnt = 0;
	for (int i = 0; i < root_cnt; i++) {

		otype* t = get_object_type(roots[i]);
		for (int j = 0; j < t->member_cnt; j++) {

			if (!IS_REFERENCE(roots[i], j)) continue;
			oref mem = convert_to_oref((void*) (resolve_object(ref) + compute_offset(t, j)), ref.heap);
			cnt += __count_references(ref, &mem, 1); // Hacky hack.

		}

	}

	return cnt;

}