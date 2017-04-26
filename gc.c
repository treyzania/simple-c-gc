#include "gc.h"

oref create_object(otype* t, heap* h) {

}

void free_object(oref ref) {

}

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
