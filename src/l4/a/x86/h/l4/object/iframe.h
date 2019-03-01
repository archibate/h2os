#pragma once

/* N: keep sync with system/asm/iframe.asm */
struct iframe
{
/* PUSHADs: */
	word_t edi, esi, ebp, old_esp, ebx, edx, ecx, eax;
/* segment regs */
	word_t gs, fs, es, ds;
/* (pushed in ents, before introute) */
/* Intrrrupt No. & Error Code */
	word_t intr_num, error_code;
/* pushed by hardware when intrrrupt occurred */
	word_t pc, cs, eflags;
#define iframe_kern_top sp
/* only intrrrupts from user level has: */
	word_t sp, ss;
}
_PACKED;
