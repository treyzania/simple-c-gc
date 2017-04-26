#include "obj.h"

void* resolve_object(oref r) {
	return r.heap != NULL ? r.heap + (r.ptr % r.heap->size) : NULL;
}

oref convert_to_oref(void* o, heap* h) {

	// This probably isn't the best way to do it.
	oref r;
	r.ptr = (uint64_t) o - (uint64_t) h;
	r.heap = h;
	return r;

}

otype* get_object_type(oref r) {
	void* o = resolve_object(r);
	return ((objheader*) o)->type;
}

uint64_t compute_type_size(otype* t) {
	return compute_offset(t, t->member_cnt);
}

uint64_t compute_offset(otype* t, uint32_t fi) {

	uint64_t s = sizeof(objheader); // Initial value
	for (uint32_t i = 0; i < fi; i++) {

		memberdef mem = t->members[i];
		if (mem.member_flags & MEMBER_ISREFERENCE) {
			s += (uint64_t) sizeof(oref);
		} else {
			primtive_type pt = mem.data.ptype;
			if (pt == PRIM_BYTE) {
				s += PRIM_BYTE_SIZE;
			} else if (pt == PRIM_INT) {
				s += PRIM_INT_SIZE;
			}
		}

	}

	return s;

}
