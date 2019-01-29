#pragma once

#include <ccutils.h>
#include <l4/types.h>
#include <l4/asm/itags.h>
#include <l4/ipcregs.h>

typedef struct IPCBuffer
{
	word_t tags[L4_IPCTags];
	union {
		word_t regs[L4_IPCRegisters];
		struct {
			word_t words[L4_IPCWords];
			cptr_t caps[L4_IPCCaps];
			word_t extraWords;
			word_t extraCaps;
		} _PACKED;
	} _PACKED;
}
_PACKED
IPCBuffer_t;
