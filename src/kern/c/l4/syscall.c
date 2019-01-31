#include <l4/syscall.h>
#include <k/printk.h>
#include <k/panic.h>

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

int _FASTCALL systemCall(cptr_t cptr, Invo_t *invo)
{
	cap_t *cap = &mycaps[cptr];

	invo->capDest = &mycaps[L4_InitCapDestSlot0];
	invo->capCount = 1;

	int res = sysInvoke(cap, invo);

	extern const char *__ntNameTableOfEnum_L4_ErrorNumber[];
	if (res < 0) dprintk("systemCall(): sysInvoke() returned %d (%s)",
			res, __ntNameTableOfEnum_L4_ErrorNumber[-res]);

	return res;
}
