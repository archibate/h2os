#include <l4/cnode.h>
#include <l4/objects.h>
#include <bittools.h>

#define CapLookupError(x)  ((cap_t*)0)

cap_t *CNode_lookup(CNode_t const *root, cptr_t cptr, bits_t depth)
{
	cap_t *cap;
	cptr_t mask;
again:
	mask = LomaskOfBits(root->guardBits);
	if ((root->guard & mask) != (cptr & mask))
		return CapLookupError(GuardDismatch);

	depth -= root->guardBits;
	cptr >>= root->guardBits;

	if (root->depth > depth)
		return CapLookupError(DepthDismatch);

	mask = LomaskOfBits(root->depth);
	cap = &root->cs[cptr & mask];

	depth -= root->depth;
	cptr >>= root->depth;

	if (!depth)
		return cap;

	switch (cap->objType)
	{
	case L4_CNodeObject:
		root = (CNode_t*)cap->ptr;
		goto again;
	default:
		return CapLookupError(TooDeep);
	}
}
