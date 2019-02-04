#pragma once

enum L4_InitCapPtr
{
	L4_InitCapNull = 0,
	L4_InitCapConsole,
	L4_InitCapIOPort,
	L4_InitCapDebug,
	L4_InitCapTCB,
	L4_InitCapCSpace,
	L4_InitCapExtra,
	L4_InitCapSigma0,
	L4_InitCapDestSlot0,
	L4_InitCaps,
	L4_InitCapMax = 512,
};

#include <stassert.h>
static void __stassert1sa345907rklhsf(void)
{
	static_assert(L4_InitCapMax > L4_InitCaps);
}
