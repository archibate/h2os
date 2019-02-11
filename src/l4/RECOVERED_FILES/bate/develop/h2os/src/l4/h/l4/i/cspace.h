#pragma once

#include <l4/types.h>
#include <l4/kobject.h>
#include <l4/capability.h>

typedef struct CSpace {
	cap_t *objptr;
	struct kobject ko;
	word_t limit;
	word_t water;
} CSpace_t;

cap_t *csLookup(CSpace_t *cs, cptr_t cptr);
cap_t *csGetDestSlot(CSpace_t *cs);
cap_t *uncref(cap_t *cap);
