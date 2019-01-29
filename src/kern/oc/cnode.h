#pragma once

#include <l4/types.h>
#include <l4/capability.h>

typedef struct CNode
{
	cap_t *cs; // size = (1 << depth)
	cptr_t guard;
	bits_t guardBits, depth;
}
CNode_t;

#include <bittools.h>
static cap_t *CNode_getAt(CNode_t const *cnode, cptr_t index)
{
	if (index < SizeOfBits(cnode->depth))
		return &cnode->cs[index];
	else
		return 0;
}

cap_t *CNode_lookup(CNode_t const *root, cptr_t cptr, bits_t depth);
