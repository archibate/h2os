#pragma once

enum L4_ContextIndex
{
	L4_Context_EDI,
	L4_Context_ESI,
	L4_Context_EBP,
	L4_Context_EBX,
	L4_Context_EDX,
	L4_Context_ECX,
	L4_Context_EAX,
	L4_Context_GS,
	L4_Context_FS,
	L4_Context_ES,
	L4_Context_DS,
	L4_Context_EIP,
	L4_Context_CS,
	L4_Context_EFLAGS,
	L4_Context_ESP,
	L4_Context_SS,
	L4_ContextWords,
/* some arch-independent shortcuts: */
	L4_Context_PC = L4_Context_EIP,
	L4_Context_SP = L4_Context_ESP,
};