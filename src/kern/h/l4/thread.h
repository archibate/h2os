#pragma once

#include <l4/types.h>
#include <l4/consts.h>
#include <l4/asm/context.h>

typedef struct TCB
{
	cap_t cspace;
	word_t extraBuf[L4_MaxExtraWords];
	word_t context[L4_ContextWords];
	word_t priority;
	struct TCB *next, *prev;
}
tcb_t;
