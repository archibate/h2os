#pragma once

#include <l4/machine/mmu/page.h>
#include <l4/object/identry.h>

struct slab
{
	void *page;
	unsigned int used;
	unsigned int rtype;
	size_t rsize;
};
