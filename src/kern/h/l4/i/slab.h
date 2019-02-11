#pragma once

#include <l4/types.h>
#include <l4/kobject.h>
#include <l4/capability.h>

typedef struct CSlab {
	struct kobject ko;
	void *objptr;
	word_t water;
	byte_t retype;
} CSlab_t;

int do_Slab_Allocate(CSlab_t *slab, cap_t *capDest, word_t num);
