#pragma once

/* N: keep sync with system/asm/seframe.asm */
struct seframe
{
	word_t eax;
	word_t es, ds;
	word_t pc, sp;
}
_PACKED;
