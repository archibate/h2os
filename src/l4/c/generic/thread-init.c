#include <l4/generic/sched.h>
#include <l4/generic/thread.h>
#include <l4/generic/idspace.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/allocpage.h>
#include <l4/generic/context.h>
#include <l4/generic/kcache.h>
#include <memory.h>

KCACHE(ktcb);

struct ktcb *new_thread(void)
{
	struct ktcb *tcb = KCNEW(ktcb);
	__thread_init(tcb);
	tcb->ipcbuf = (void*)alloc_page();
	return tcb;
}

void __thread_init(struct ktcb *tcb)
{
	memset(tcb, 0, sizeof(*tcb));
	context_init(&tcb->context);
}

void thread_init(struct ktcb *tcb)
{
	__thread_init(tcb);

	tcb->mm = current->mm;
	tcb->ipcbuf = current->ipcbuf;
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
	//printk("!!!");
	IDDEL(tcb);
	KCDEL(ktcb, tcb);
	//hlist_del(&tcb->hlist);
	//list_del(&tcb->list);
}
