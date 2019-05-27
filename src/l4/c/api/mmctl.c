#include <l4/api/mmctl.h>
#include <l4/generic/sched.h>
#include <l4/generic/mman.h>
#include <l4/generic/thread.h>
#include <l4/generic/kcache.h>
#include <l4/generic/idspace.h>
#include <l4/enum/errno.h>
#include <l4/enum/thread-states.h>
#include <l4/misc/bug.h>

KCACHE(mm);

sl4id_t sys_create_mm(l4id_t mmc)
{
	struct mm *src_mm = mmc == -1 ? current->replying->mm : LID(current->mm, mm, mmc);
	if (!src_mm)
		return -ESRCH;
	struct mm *mm = KCNEW(mm);
	mm_init(mm);
	//mm_copy_fds(mm, src_mm);
	src_mm->num_children++;
	mm->parent = src_mm;
	return LIDNEW(current->mm, mm, mm);
}

int sys_mm_btw_fdup2(l4id_t msrc, l4fd_t fsrc, l4id_t mdst, l4fd_t fdst)
{
	struct mm *src_mm = msrc == -1 ? current->replying->mm : LID(current->mm, mm, msrc);
	struct mm *dst_mm = mdst == -1 ? current->replying->mm : LID(current->mm, mm, mdst);
	if (!src_mm || !dst_mm)
		return -ESRCH;
	//printk("mm_copy_fd: %d/%d->%d/%d", msrc, fsrc, mdst, fdst);
	return mm_copy_fd(dst_mm, src_mm, fdst, fsrc);
}

sl4id_t sys_mm_new_thread(l4id_t mmc, uintptr_t pc, uintptr_t sp)
{
	struct mm *mm = LID(current->mm, mm, mmc);
	if (!mm)
		return -ESRCH;
	struct ktcb *tcb = new_thread();
	tcb->context.pc = pc;
	tcb->context.sp = sp;

	tcb->state = THREAD_RUNNING;//?
	thread_active(tcb);//?

	tcb->mm = mm;
	//printk("sys_mm_new_thread(%p)", tcb);
	return LIDNEW(mm, ktcb, tcb);
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
