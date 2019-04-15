#pragma once

#include <l4/lib/list.h>
#include <l4/enum/rtype.h>
#include <l4/object/utcb.h>
#include <l4/object/ipcbuf.h>
#include <l4/object/identry.h>
#include <l4/object/fdentry.h>
#include <l4/object/context.h>
#include <l4/object/msginfo.h>
#include <l4/object/endpoint.h>
#include <l4/object/mman.h>
#include <l4/enum/filedes.h>

struct ktcb
{
	struct list_node list;
	struct hlist_node hlist;

	struct ids_entry ide;

	unsigned char priority;
	unsigned char state;
	//unsigned char ipcphase;

	struct context context;

	struct ktcb *replying;

	struct msginfo *prplmip;
	struct msginfo *psndmip;
	struct msginfo msginfo;

	struct endpoint ep;

	struct ipc_buf *ipcbuf;

	// T: below move to kpcb and mm mm
	struct mm *mm;

	l4fd_t fdtop;
	struct fd_entry fds[MAX_FDS]; // T: maybe use hash table better?
};
