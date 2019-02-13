#pragma once

#include <l4/lib/list.h>
#include <l4/enum/rtype.h>
#include <l4/object/identry.h>

struct ktcb
{
	struct list_node list;
	struct hlist_node hlist;

	struct ids_entry ide;

	unsigned char state;
	unsigned char priority;

	struct utcb *utcb;
	struct pgdir *pgdir;
};
