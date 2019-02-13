#include <l4/generic/sched.h>
#include <l4/generic/thread.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/utcb.h>
#include <l4/boot/alloc.h>
#include <memory.h>

void __thread_init(struct ktcb *tcb)
{
	memset(tcb, 0, sizeof(*tcb));
}

void thread_init(struct ktcb *tcb)
{
	__thread_init(tcb);

	struct utcb *utcb = calloc(1, PageSize);
	utcb_init(utcb);
	tcb->utcb = utcb;
	tcb->pgdir = current->pgdir;
}

void thread_revoke(struct ktcb *tcb)
{
	//utcb_deref(&tcb->utcb);
	//pgdir_deref(&tcb->pgdir);
}

void thread_delete(struct ktcb *tcb)
{
	thread_revoke(tcb);
	hlist_del(&tcb->ide.hlist);
}
