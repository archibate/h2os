#pragma once

#include <l4/capability.h>
#include <l4/types.h>

cap_t *capLookup(cptr_t cptr);
cap_t *capGetDestSlot(void);
