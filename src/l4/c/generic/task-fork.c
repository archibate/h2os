#include <l4/generic/task-fork.h>
#include <l4/generic/kcget.h>
#include <l4/generic/mman.h>
#include <l4/generic/sched.h>
#include <l4/generic/task-switch.h>
#include <l4/generic/allocpage.h>
#include <l4/enum/rtype.h>
#include <memory.h>

struct ktcb *task_fork_current(void)
{
	struct mm *new_mm = mm_fork(current->mm);
	if (!new_mm)
		return NULL;

	struct ktcb *tcb = kcg_new(RTYPE_THREAD);
	task_newly_forked(tcb);//i.e.task_fork_context
	tcb->mm = new_mm;
	tcb->ipcbuf = (void*)alloc_page();
	tcb->fdtop = current->fdtop;
	memcpy(tcb->fds, current->fds, sizeof(tcb->fds));
	tcb->priority = current->priority;
	return tcb;
}
