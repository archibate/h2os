#include <l4/generic/sched.h>
#include <l4/generic/thread.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/allocpage.h>
#include <l4/generic/utcb.h>
#include <memory.h>

void __thread_init(struct ktcb *tcb)
{
	memset(tcb, 0, sizeof(*tcb));
}

void thread_init(struct ktcb *tcb)
{
	__thread_init(tcb);

	struct utcb *utcb = alloc_page();
	utcb_init(utcb);
	tcb->utcb = utcb;
	tcb->pgdir = current->pgdir;
}

void thread_revoke(struct ktcb *tcb)
{
	//utcb_deref(&tcb->utcb);
	//pgdir_deref(&tcb->pgdir);
	//free_page(tcb->utcb);
}

void thread_delete(struct ktcb *tcb)
{
	thread_revoke(tcb);
	hlist_del(&tcb->ide.hlist);
	//hlist_del(&tcb->hlist);
	//list_del(&tcb->list);
}
