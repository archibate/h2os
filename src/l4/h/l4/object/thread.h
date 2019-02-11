#pragma once

#include <l4/lib/list.h>
#include <l4/enum/rtype.h>

struct ktcb
{
	struct list_node list;
	struct hlist_node hlist;

	unsigned char state;
	unsigned char priority;

	struct utcb *utcb;
	struct pgdir *pgdir;

	struct id_space *idspaces[RTYPE_MAX];
};
