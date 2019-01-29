#pragma once

#define L4_IPCWords   16
#define L4_IPCCaps    16

enum
{
	L4_IPCReg_Word0      = 0,
	L4_IPCReg_Cap0       = L4_IPCWords,
	L4_IPCReg_ExtraWords = L4_IPCWords + L4_IPCCaps,
	L4_IPCReg_ExtraCaps,
	L4_IPCRegisters,
};
