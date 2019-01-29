#include <k/kstack.h>
#include <k/printk.h>
#include <k/panic.h>
#include <l4/types.h>

void hwsyscall(void)
{
	panic("hwsyscall()");

#if 0
	cptr_t cap = kIFrame[IFrame_EAX];
	cptr_t c1 = kIFrame[IFrame_EBX];
	ulong a1 = kIFrame[IFrame_ESI];
	ulong a2 = kIFrame[IFrame_EDI];
	ulong a3 = kIFrame[IFrame_EBP];
	systemInvoke(cap, c1, a1, a2, a3);
#endif
}
