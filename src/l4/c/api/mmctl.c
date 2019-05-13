#include <l4/generic/sched.h>
#include <l4/generic/mman.h>
#include <l4/api/mmctl.h>
//#include <l4/generic/softfault.h>
#include <l4/generic/thread.h>
#include <l4/enum/errno.h>
#include <l4/enum/thread-states.h>
#include <l4/misc/bug.h>

int sys_mm_new_thread(l4id_t mmc, uintptr_t pc, uintptr_t sp)
{
	struct mm *mm = LID(current->mm, mm, mmc);
	if (!mm)
		return -ESRCH;
	struct ktcb *proc = thread_new();
	proc->context.pc = pc;
	proc->context.sp = sp;
	return LIDNEW(mm, proc);
}

int sys_mm_destroy(l4id_t mmc)
{
	struct mm *mm = LID(current->mm, mm, mmc);
	if (!mm)
		return -ESRCH;
	mm_destroy(mm);
	LIDDEL(mm);
	return 0;
}
