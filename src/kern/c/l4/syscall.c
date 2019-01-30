#include <l4/syscall.h>
#include <k/printk.h>
#include <k/panic.h>

#include <l4/objects.h>
#include <l4/inicaps.h>
#include <l4/offsets.h>
cap_t mycaps[L4_InitialCaps];
void setup_mycaps(void)
{
	mycaps[L4_InitialNullCap] = (cap_t)
	{
		.objType = L4_NullObject,
		.object = 0,
		.base = 0,
		.limit = 0,
	};
	mycaps[L4_InitialConsoleCap] = (cap_t)
	{
		.objType = L4_ConsoleObject,
		.object = 0,
		.base = 0,
		.limit = 1,
	};
	mycaps[L4_InitialIOPortCap] = (cap_t)
	{
		.objType = L4_IOPortObject,
		.object = 0,
		.base = 0x0,
		.limit = 0x1000,
	};
	mycaps[L4_InitialDebugCap] = (cap_t)
	{
		.objType = L4_DebugObject,
		.object = 0,
		.base = 0,
		.limit = L4_Debug_MaxOffset,
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
