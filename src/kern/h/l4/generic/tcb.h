#pragma once

#include <l4/lib/list.h>

struct ktcb
{
	struct list_node list;
	struct hlist_node hlist;
};
