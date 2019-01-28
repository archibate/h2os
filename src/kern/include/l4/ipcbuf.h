#pragma once

#include <ccutils.h>
#include <l4/types.h>
#include <l4/asm/itags.h>

#define IPCCaps       16
#define IPCWords      16

typedef struct IPCBuffer
{
	word_t tags[IPCTags];
	union {
		word_t regs[0];
		struct {
			word_t words[IPCWords];
			cptr_t caps[IPCCaps];
			word_t extraWords;
			word_t extraCaps;
		} _PACKED;
	} _PACKED;
}
_PACKED
IPCBuffer_t;

#define IPCRegisters  (sizeof(IPCBuffer_t) / sizeof(word_t) - IPCTags)
