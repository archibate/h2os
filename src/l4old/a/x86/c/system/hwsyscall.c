#include <l4/system/kstack.h>
#include <l4/misc/panic.h>

void hwsysintr(void)
{
	panic("hwsysintr");
}

void hwsysenter(void)
{
	panic("hwsysenter");
}
