#pragma once

#include <l4/types.h>
#include <l4/consts.h>
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
	cap_t cspace;
	byte_t state;
	byte_t priority;
	//cap_t replySlot;
	word_t extraBuf[L4_MaxExtraWords];
	word_t context[L4_ContextWords];
}
tcb_t;
