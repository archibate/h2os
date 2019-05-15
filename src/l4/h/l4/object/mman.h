#pragma once

#include <l4/object/mregion.h>
#include <l4/object/fdentry.h>
#include <l4/object/idspace.h>
#include <l4/object/identry.h>
#include <l4/object/endpoint.h>
#include <l4/enum/filedes.h>

struct mm
{
	struct ids_entry ide;
	struct ids_entry lide;

	struct id_space lids_ktcb;
	struct id_space lids_mm;

	struct hlist_head mregs;
	struct pgdir *pgdir;

	struct mm *parent;
	struct endpoint ep_chld;
	size_t num_children;

	//struct id_space lids_fd_entry;
	l4fd_t fdtop;
	struct fd_entry fds[MAX_FDS]; // T: maybe use hash table better?
};
