#pragma once

#include <l4/lib/list.h>
#include <l4/enum/rtype.h>
#include <l4/object/utcb.h>
#include <l4/object/ipcbuf.h>
#include <l4/object/identry.h>
#include <l4/object/fdentry.h>
#include <l4/object/context.h>
#include <l4/enum/filedes.h>

struct ktcb
{
	struct list_node list;
	struct hlist_node hlist;

	struct ids_entry ide;

	unsigned char state;
	unsigned char priority;

	struct ktcb *replying;

	struct ipc_buf *ipcbuf;
	struct context context;

	// T: below move to kpcb
	struct pgdir *pgdir;

	l4fd_t fdtop;
	struct fd_entry fds[MAX_FDS];
};
