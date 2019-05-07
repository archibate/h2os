#include <l4/generic/sched.h>
#include <l4/generic/mman.h>
#include <l4/api/mmctl.h>
#include <l4/api/sched.h>
#include <l4/generic/softfault.h>
#include <l4/enum/errno.h>

int sys_mmctl_setpcsp(l4id_t mmc, uintptr_t pc, uintptr_t sp)
{
	if (mmc != -1)
		return -EINVAL;
	current->replying->context.pc = pc;
	current->replying->context.sp = sp;
	return 0;
}

int sys_mmctl_destroy(l4id_t mmc)
{
	if (mmc != -1)
		return -EINVAL;
	mm_destroy(current->replying->mm);
	return 0;
}

sl4id_t sys_fork(void)
{
	struct ktcb *tcb = softfault_fork_and_tcb(0);
	if (tcb == NULL)
		return -ENOMEM;
	hlist_add_head(&tcb->hlist_child, &current->children);
	tcb->context.eax = 0;
	return idg_new_entry(&tcb->ide, RTYPE_THREAD);
}
