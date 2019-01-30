#include <l4/syscall.h>
#include <k/printk.h>
#include <k/panic.h>

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
}

int _FASTCALL systemCall(cptr_t cptr, Invo_t *invo)
{
	cap_t *cap = &mycaps[cptr];
	int res = sysInvoke(cap, invo);

	extern const char *__ntNameTableOfEnum_L4_ErrorNumber[];
	if (res) dprintk("systemCall(): sysInvoke() returned %d (%s)", res, __ntNameTableOfEnum_L4_ErrorNumber[-res]);

	return res;
}
