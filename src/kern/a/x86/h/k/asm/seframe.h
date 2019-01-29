#pragma once

/* N: keep sync with isr/seframe.asm */
enum SEFrameIndex {
	SEFrame_ES,
	SEFrame_DS,
/* the total length: */
	SEFrame_EIP,
	SEFrame_ESP,
	SEFrameWords,
/* some easy to use shortcuts: */
	SEFrame_PC = SEFrame_EIP,
	SEFrame_SP = SEFrame_ESP,
};
