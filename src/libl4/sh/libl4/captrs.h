#pragma once

#include <l4/types.h>
#include <l4/inicaps.h>

enum Libl4_CapPtr
{
	Libl4_CapCSpace  = L4_InitCapCSpace,
	Libl4_CapPgdir   = L4_InitCapPgdir,
	Libl4_CapMemory  = L4_InitCapSigma0,
	Libl4_CapConsole = L4_InitCapConsole,
	Libl4_CapDebug   = L4_InitCapDebug,
	Libl4_CapExtra   = L4_InitCapExtra,

	Libl4_CapDestSlot0 = L4_InitCapDestSlot0,
	Libl4_CapDestSlot1,
	Libl4_CapDestSlot2,
	Libl4_CapSlab0,
	Libl4_CapPgdir0,
	Libl4_CapPgtab0,
	Libl4_CapPage0,
	Libl4_CapPage1,
	Libl4_CapTCB0,
};
