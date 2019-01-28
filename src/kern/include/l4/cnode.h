#pragma once

#include <ccutils.h>
#include <l4/types.h>
#include <l4/capability.h>

typedef struct CNode
{
	cap_t *cs; // size = (1 << depth)
	cptr_t guard;
	bits_t guardBits, depth;
}
CNode_t;

cap_t *cgetEx(CNode_t const *root, cptr_t cptr, bits_t depth);
#define cget(root, cptr) cgetEx(root, cptr, 32)
