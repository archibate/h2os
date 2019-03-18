#pragma once

struct context
{
	word_t edi, esi, ebp, ebx, edx, ecx, eax;
	word_t pc, sp, eflags;
};

#define ctx_r0 eax
#define ctx_r1 ecx
#define ctx_r2 edx
