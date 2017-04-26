#include "obj.h"

void* resolve_object(oref r) {
	return r.heap != NULL ? r.heap + (r.ptr % r.heap->size) : NULL;
}

otype* get_object_type(oref r) {
	void* o = resolve_object(r);
	return ((objheader*) o)->type;
}

uint64_t compute_type_size(otype* t) {

	uint64_t total = sizeof(objheader); // Initial value
	for (uint32_t i = 0; i < t->member_cnt; i++) {

		memberdef mem = t->members[i];
		member_family f = mem.family;
		if (f == PRIMITIVE) {

			primtive_type p = mem.data.ptype;
			if (p == PRIM_BYTE) {
				total += sizeof(uint8_t);
			} else if (p == PRIM_INT) {
				total += sizeof(uint32_t);
			}

		} else if (f == OBJECT) {

			if (mem.data.otype.ref_cnt == 0) {
				/*
				 * FIXME The issue with this is that there's no way to fit a
				 *       child object (with more members) into one of these
				 *       members without indirection of some sort.
				 */
				total += compute_type_size(mem.data.otype.type);
			} else {
				total += sizeof(oref);
			}

		}

	}

}
