#pragma once

#include <l4/object/mregion.h>

struct mm
{
	struct hlist_head mregs;
	struct id_space lids_fd_entry;
	struct id_space lids_ktcb;
	struct pgdir *pgdir;
	char buf[245];
};
