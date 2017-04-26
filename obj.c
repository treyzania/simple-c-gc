#include "obj.h"
#include <glib.h>

oref create_object(heap* h, otype* t) {



}

void* resolve_object(oref r) {
	return r.heap != NULL ? r.heap + (r.ptr % r.heap->size) : NULL;
}

otype* get_object_type(oref r) {
	void* o = resolve_object(r);
	return ((objheader*) o)->type;
}

void main(void) {
	// Do nothing right now.
}
