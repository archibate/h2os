#pragma once

#include <l4/lib/list.h>

struct ids_entry
{
	struct hlist_node hlist;
	l4id_t id;
};
