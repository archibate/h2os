#pragma once

#include <l4/lib/list.h>
#include <l4/object/space.h>

struct ktcb
{
	struct list_node list;
	struct hlist_node hlist;

	unsigned char state;
	unsigned char priority;

	struct space space;
	struct utcb *utcb;
	struct pgdir *pgdir;
};
