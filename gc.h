#pragma once
#include "obj.h"
#include "heap.h"

oref create_object(otype* t, heap* h);
void free_object(oref ref);
