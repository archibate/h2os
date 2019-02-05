#include <l4/cspace.h>
#include <l4/captypes.h>
#include <assert.h>

cap_t *csLookup(cap_t *cs, cptr_t cptr)
{
	if (cptr > cs->c_limit)
		return 0;
	return &((cap_t*)cs->c_objptr)[cptr];
}

cap_t *csGetDestSlot(cap_t *cs)
{
	assert(cs->c_water < cs->c_limit);
	return &((cap_t*)cs->c_objptr)[cs->c_water];
}

cap_t *uncref(cap_t *cap)
{
	if (!cap)
		return cap;
	if (cap->c_type == L4_CRefCap)
		cap = cap->c_objptr;
	assert(cap->c_type != L4_CRefCap);
	return cap;
}
