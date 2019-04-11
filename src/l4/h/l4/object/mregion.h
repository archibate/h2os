#pragma once

#include <l4/object/fdentry.h>
#include <l4/lib/list.h>

struct mregion
{
	struct hlist_node hlist;
	word_t start, end;
	struct fd_entry fde;
};
