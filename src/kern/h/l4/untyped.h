#pragma once

#include <l4/types.h>
#include <l4/cnode.h>

typedef struct Untyped
{
	void *base;
	size_t used, size;
}
Untyped_t;

size_t Untyped_Retype(Untyped_t *unt,
		word_t type, bits_t sizeBits,
		word_t num, word_t offset, CNode_t *root);
