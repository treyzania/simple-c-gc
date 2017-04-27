#pragma once

#include <stdint.h>

#include "heap.h"

typedef uintptr_t optr;

struct oref_t;
typedef struct oref_t oref;
struct otype_t;
typedef struct otype_t otype;
struct objheader_t;
typedef struct objheader_t objheader;
struct memberdef_t;
typedef struct memberdef_t memberdef;

struct oref_t {
	optr ptr;
	heap* heap;
};

#define OREF_EQUAL(a, b) (a.heap == b.heap && a.ptr == b.ptr)

#define TYPE_FLAG_ANONYMOUS (1 << 0) // The name field is NULL.

struct otype_t {
	size_t size;
	uint64_t flags;
	char* name;
	uint32_t member_cnt;
	memberdef* members;
};

#define OBJ_NORMAL 0
#define OBJ_RAW (1 << 0)

struct objheader_t {
	otype* type;
	uint64_t objectflags; // If 0, then just the thing itself.
};

typedef enum { PRIM_BYTE, PRIM_INT } primtive_type;

#define MEMBER_ISREFERENCE (1 << 0)
#define PRIM_BYTE_SIZE 1
#define PRIM_INT_SIZE 4

#define IS_REFERENCE(o, i) (((objheader*) resolve_object(o))->type->members[i].member_flags & MEMBER_ISREFERENCE != 0)

struct memberdef_t {
	uint64_t member_flags;
	union {
		primtive_type ptype; // Primitive type
		otype* rtype; // Objects
	} data;
};

void* resolve_object(oref r);
oref convert_to_oref(void* o, heap* h);
otype* get_object_type(oref r);

uint64_t compute_type_size(otype* t);
uint64_t compute_offset(otype* t, uint32_t fi);
