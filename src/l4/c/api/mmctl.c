#include <l4/generic/sched.h>
#include <l4/api/mmctl.h>
#include <l4/enum/errno.h>

int sys_mmctl_setpc(l4id_t mmc, uintptr_t pc)
{
	if (mmc != -1)
		return -EINVAL;
	current->replying->context.pc = pc;
	return 0;
}
