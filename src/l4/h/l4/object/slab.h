#pragma once

#include <l4/lib/list.h>

struct slab
{
	struct hlist_head f;
	size_t rsize;
};
