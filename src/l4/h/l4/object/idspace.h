#pragma once

#include <l4/lib/list.h>

#define ID_HASH_MAX    29

struct id_space
{
	struct hlist_head ht[ID_HASH_MAX];
};
