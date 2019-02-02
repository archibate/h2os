#pragma once

#include <l4/types.h>
#include <l4/consts.h>
#include <l4/asm/context.h>

typedef struct TCB
{
	word_t extraBuf[L4_MaxExtraWords];
	word_t context[L4_ContextWords];
}
tcb_t;
