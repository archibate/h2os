#pragma once

#include <l4/capability.h>
#include <l4/types.h>

cap_t *csLookup(CCSpace_t *cs, cptr_t cptr);
cap_t *csGetDestSlot(CCSpace_t *cs);
cap_t *uncref(cap_t *cap);
