#pragma once

#include <l4/cnode.h>
#include <l4/ipcbuf.h>

typedef struct TCB
{
	CNode_t root;
	IPCBuffer_t ipcbuf;
}
tcb_t;
