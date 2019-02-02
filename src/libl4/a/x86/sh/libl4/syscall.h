#pragma once

#include <l4/types.h>
#include <l4/asm/shortmsg.h>

static l4Ret_t l4Syscall(l4CPtr_t cptr, l4Word_t shrt[L4_ShortMsgWords])
{
	int res;
	asm volatile (
		"push %%ebp\n"
		"mov %%edx, %%ebp\n"
		"call libl4_do_syscall\n"
		"pop %%ebp\n"
		: "=a" (res)
		: "a" (cptr)
		, "b" (shrt[L4_ShortMsg_EBX])
		, "D" (shrt[L4_ShortMsg_EDI])
		, "S" (shrt[L4_ShortMsg_ESI])
		, "d" (shrt[L4_ShortMsg_EBP])
		: "cc", "memory");
	return res;
}
