#pragma once

#include <l4/types.h>
#include <k/asm/iframe.h>
#include <k/asm/seframe.h>
#include <ccutils.h>

typedef struct UTCB
{
	word_t seframe[SEFrameWords];
	word_t iframe[IFrameWords];
}
_PACKED
utcb_t;

void UTCB_Init(utcb_t *utcb, word_t pc, word_t sp);
#define Arch_InitUTCB(utcb) UTCB_Init(utcb, 0, 0)
