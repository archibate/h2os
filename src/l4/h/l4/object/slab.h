#pragma once

#include <l4/machine/mmu/page.h>
#include <l4/object/identry.h>
#include <l4/enum/rtype.h>

struct slab
{
	void *page;
	size_t used;
	size_t rsize;
	unsigned int rtype;
};
