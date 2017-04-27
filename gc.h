#pragma once

#include <glib.h>

#include "obj.h"
#include "heap.h"

typedef struct {

	GList* oarray;
	heap* mem;
	optr root_offset;

} objheap;

objheap* create_objheap();
void destroy_objheap(objheap* oh);

oref create_object(otype* t, objheap* h);
void free_object(oref ref);
