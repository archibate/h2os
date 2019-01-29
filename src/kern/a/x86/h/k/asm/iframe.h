#pragma once

/* N: keep sync with isr/iframe.asm */
enum IFrameIndex {
/* PUSHADs: */
	IFrame_EDI = 0,
	IFrame_ESI,
	IFrame_EBP,
	IFrame_OldESP,
	IFrame_EBX,
	IFrame_EDX,
	IFrame_ECX,
	IFrame_EAX,
/* segment regs */
	IFrame_GS,
	IFrame_FS,
	IFrame_ES,
	IFrame_DS,
/* pushed in ents, before introute */
/* Intrrrupt No. & Error Code */
	IFrame_IntrNum,
	IFrame_ErrorCode,
/* pushed by hardware when intrrrupt occurred */
	IFrame_EIP,
	IFrame_CS,
	IFrame_EFLAGS,
/* only intrrrupts from user level has: */
	IFrame_ESP,
	IFrame_SS,
/* the total length: */
	IFrameWords,
/* some easy to use shortcuts: */
	IFrame_PC = IFrame_EIP,
	IFrame_SP = IFrame_ESP,
};
