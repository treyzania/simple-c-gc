#pragma once

#include <glib.h>

#include "obj.h"
#include "heap.h"

typedef struct {
	heap* mem;
	GList* allocs;
	optr root_offset;
} objheap;

typedef struct {
	optr offset;
	uint8_t flags;
} allocref;

#define OA_NORMAL 0

objheap* create_objheap();
void destroy_objheap(objheap* oh);

oref create_object(otype* t, objheap* h);
void free_object(oref ref);

oref copy_object(oref ref, heap* dest);
