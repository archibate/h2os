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
		CNode_t *root, word_t offset,
		word_t type, size_t sizeMaybe,
		word_t num);
