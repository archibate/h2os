#include <l4/syscall.h>
#include <l4/capability.h>
#include <l4/invoke.h>
#include <k/printk.h>
#include <k/panic.h>

#include <l4/consts.h>
static word_t extraMsgBuf[L4_MaxExtraWords];

#include <k/kbase.h>
#include <l4/captypes.h>
#include <l4/inicaps.h>
cap_t mycaps[L4_InitCaps];
void setup_mycaps(void)
{
	mycaps[L4_InitCapNull] = (cap_t)
	{
		.c_type = L4_NullCap,
	};
	mycaps[L4_InitCapConsole] = (cap_t)
	{
		.c_type = L4_ConsoleCap,
	};
	mycaps[L4_InitCapIOPort] = (cap_t)
	{
		.c_type = L4_IOPortCap,
		.c_base = 0x0,
		.c_limit = 0x1000,
	};
	mycaps[L4_InitCapDebug] = (cap_t)
	{
		.c_type = L4_DebugCap,
	};
	mycaps[L4_InitCapExtra] = (cap_t)
	{
		.c_type = L4_ExtraCap,
		.c_objptr = extraMsgBuf,
		//.c_base = extraMsgBuf,
		.c_limit = L4_MaxExtraWords,
		.c_water = 0,
	};
	mycaps[L4_InitCapSigma0] = (cap_t)
	{
		.c_type = L4_SegmentCap,
		.c_base = KernSigma0Begin,
		.c_limit = KernSigma0End - KernSigma0Begin,
		.c_water = 0,
	};
	mycaps[L4_InitCapDestSlot0] = (cap_t)
	{
		.c_type = L4_NullCap,
	};
}

int _FASTCALL systemCall(cptr_t cptr, word_t *shortMsg)
{
	cap_t *cap = &mycaps[cptr];
	cap_t *capDest = &mycaps[L4_InitCapDestSlot0];

	extern const char *__ntNameTableOfEnum_L4_CapType[];
	extern const char *__ntNameTableOfEnum_L4_InitCapPtr[];
	extern const char *__ntNameTableOfEnum_L4_ServiceNumber[];
	if (cptr != L4_InitCapExtra) dprintk("%s<%s$%x@%x[%x:%x%%%x]>: %s(%d)[%.8s%.8s]",
			10+__ntNameTableOfEnum_L4_InitCapPtr[cptr],
			3+__ntNameTableOfEnum_L4_CapType[cap->c_type], cap->c_retype,
			cap->c_objptr, cap->c_base, cap->c_limit, cap->c_water,
			3+__ntNameTableOfEnum_L4_ServiceNumber[shortMsg[0]],
			shortMsg[1], shortMsg+2, shortMsg[2]&0xff?extraMsgBuf:(void*)"");
	int res = sysInvoke(cap, capDest, shortMsg, extraMsgBuf);

	extern const char *__ntNameTableOfEnum_L4_ErrorNumber[];
	if (res < 0) panic("sysInvoke returned %d (%s)",
			res, __ntNameTableOfEnum_L4_ErrorNumber[-res]);
	//else dprintk("returning %d", res);

	return res;
}
