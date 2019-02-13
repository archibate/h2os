#pragma once

#include <l4/lib/list.h>

struct slab_node
{
	struct hlist_node hlist;

	void *page;

	unsigned int usage;
	char bitmap[0];
};

struct kcache
{
	struct hlist_head slab_full;
	struct hlist_head slab_partial;
	struct hlist_head slab_empty;

	size_t rsize;
	size_t rcount;
	size_t pgsize;
};
