#include <l4/cspace.h>
#include <l4/captypes.h>
#include <assert.h>

cap_t *csLookup(CCSpace_t *cs, cptr_t cptr)
{
	if (cptr > cs->limit)
		return 0;
	return &((cap_t*)cs->objptr)[cptr];
}

cap_t *csGetDestSlot(CCSpace_t *cs)
{
	assert(cs->water < cs->limit);
	return &((cap_t*)cs->objptr)[cs->water];
}

cap_t *uncref(cap_t *cap)
{
	if (!cap)
		return cap;
	if (cap->ctype == L4_CRefCap)
		cap = cap->c_objptr;
	assert(cap->ctype != L4_CRefCap);
	return cap;
}
