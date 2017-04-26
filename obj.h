#pragma once
#include <stdint.h>
#include "heap.h"

typedef uint64_t optr;

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

#define TYPE_FLAG_ANONYMOUS (1 << 0)

struct otype_t {
	size_t size;
	uint64_t flags;
	char* name;
	uint32_t member_cnt;
	memberdef* members;
};

struct objheader_t {
	otype* type;
	uint64_t objectflags; // If 0, then just the thing itself.
};

typedef enum { PRIMITIVE, OBJECT } member_family;
typedef enum { PRIM_BYTE, PRIM_INT } primtive_type;

struct memberdef_t {
	uint64_t member_flags;
	member_family family;
	union {
		primtive_type ptype; // Primitive type
		struct {
			otype* type;
			uint8_t ref_cnt; // Number of asterisks to get the actual object.
		} otype; // Objects
	} data;
};

void* resolve_object(oref r);
otype* get_object_type(oref r);

uint64_t compute_type_size(type* t);
