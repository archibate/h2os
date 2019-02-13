#pragma once

#include <l4/lib/list.h>

struct slab
{
	void *begin, *curr, *end;
	size_t rsize;
};
