#pragma once

#include <l4/lib/list.h>

#define ID_HASH_MAX    29

struct ids_entry
{
	struct hlist_node hlist;
	l4id_t id;
	void *p;
};

struct id_space
{
	struct hlist_head ht[ID_HASH_MAX];
};
