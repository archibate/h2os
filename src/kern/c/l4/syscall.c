#include <l4/syscall.h>
#include <l4/objects.h>
#include <l4/services.h>
#include <k/printk.h>
#include <k/panic.h>

int _FASTCALL systemCall(cptr_t cptr, Invo_t *invo)
{
	cap_t cap;
	cap.objType = L4_ConsoleObject;
	cap.object = 0;
	cap.base = 0;
	cap.limit = 10;

	int res = sysInvoke(&cap, invo);
	panic("systemCall(): sysInvoke() returned %d", res);
	return res;
}
