#pragma once

#include <l4/types.h>
#include <l4/consts.h>
#include <l4/kobject.h>
#include <l4/i/cspace.h>
#include <list.h>

enum TCBState
{
	TCB_NullState = 0,
	TCB_Running,
	TCB_OnSend,
	TCB_OnRecv,
	TCB_OnCall,
	TCB_Waiting,
	TCB_Suspend,
};

typedef struct tcb
{
	struct list_node list;
	struct kobject ko;
	CSpace_t t_cspace;
	//CPage_t t_utcb;
	//CPgdir_t t_pgdir;
	byte_t state;
	byte_t priority;
	word_t extraBuf[L4_MaxExtraWords];
	//word_t context[L4_ContextWords];
}
tcb_t;

/*#define t_pgdir  caps[L4_TCBCap_Pgdir]  // L4_PgdirCap
#define t_utcb   caps[L4_TCBCap_UTCB]   // L4_PageCap
#define t_cspace caps[L4_TCBCap_CSpace] // L4_CSPaceCap
#define t_pgdirAddr t_pgdir.c_objaddr
#define t_utcbAddr  t_utcb.c_objaddr*/
