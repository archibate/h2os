#pragma once

struct context
{
	word_t edi, esi, ebp, ebx, edx, ecx, eax;
	word_t pc, sp, eflags;
};
