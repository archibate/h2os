#pragma once

#include <l4/types.h>
#include <l4/consts.h>
#include <l4/tcbcaps.h>
#include <l4/capability.h>
#include <l4/asm/context.h>
#include <l4/queue.h>

enum TCBState
{
	TCB_NullState = 0,
	TCB_Running,
	TCB_OnSend,
	TCB_OnRecv,
	TCB_OnCall,
	TCB_Waiting,
};

typedef struct TCB
{
	node_t node;
	cap_t caps[L4_TCBCapsMax];
	byte_t state;
	byte_t priority;
	word_t extraBuf[L4_MaxExtraWords];
	//word_t context[L4_ContextWords];
}
tcb_t;

#define t_pgdir  caps[L4_TCBCap_Pgdir]  // L4_PgdirCap
#define t_utcb   caps[L4_TCBCap_UTCB]   // L4_PageCap
#define t_cspace caps[L4_TCBCap_CSpace] // L4_CSPaceCap
#define t_pgdirAddr t_pgdir.c_objaddr
#define t_utcbAddr  t_utcb.c_objaddr
