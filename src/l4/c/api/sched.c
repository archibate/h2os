#include <l4/api/sched.h>
#include <l4/generic/sched.h>
#include <l4/generic/task-switch.h>
#include <l4/generic/thread.h>
#include <l4/generic/idget.h>
//#include <l4/generic/task-fork.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/endpoint.h>
#include <l4/enum/errno.h>
#include <l4/misc/bug.h>

l4id_t sys_getpid(void)
{
	return current->ide.id;
}

/*static void on_wait_exit_reply(void)
{
	current->state = THREAD_SUSPEND;
	thread_suspend(current);
	thread_delete(current);
}*/

int sys_exit(void)
{
	//printk("sys_exit(%p)", current);
	current->state = THREAD_ZOMBIE;
	thread_suspend(current);
	while (NULL != endpoint_call(&current->ep_death, current, true, false));
	BUG_ON(sched_get_curr() == current);
	//current->we_callback = on_wait_exit_reply;//TODO
	//endpoint_call(&current->ep_death, current, true, false);
	//printk("sys_exited!");
	/*BUG_ON(current->state != THREAD_RUNNING);//
	current->state = THREAD_SUSPEND;
	thread_suspend(current);
	thread_delete(current);
	BUG_ON(sched_get_curr() == current);*/
	return 0;
}

#define WIN
int sys_wait_first(void)
{
	if (hlist_empty(&current->children))
		return 0;
	struct ktcb *tcb = hlist_entry(__hlist_pop(&current->children), struct ktcb, hlist_child);
	endpoint_wait(&tcb->ep_death, current, true);
	//printk("sys_wait~~~");
#ifdef WIN
	if (tcb->state == THREAD_SUSPEND) {
		tcb->state = THREAD_RUNNING;
		thread_active(tcb);
	}
#endif
	return hlist_empty(&current->children) ? 1 : 2;
}

#if 0
#include <l4/system/kstack.h>
sl4id_t sys_fork(void)
{
	//printk("%p", kErnelExiter);
	/*if (1) {
		printk("goting-fortic:");
		printk("(pc=%p)", current->context.pc);
		printk("(sp=%p)", current->context.sp);
		printk("(sp[1]=%p)", ((void**)current->context.sp)[1]);
	}*/
	struct ktcb *tcb = task_fork_current();
	if (tcb == NULL)
		return -ENOMEM;
	BUG_ON(tcb->mm->pgdir == current->mm->pgdir);
#ifndef WIN
	tcb->state = THREAD_RUNNING;
	//tcb->priority = current->priority >= 255 ? current->priority : current->priority + 1;
	thread_active(tcb);
	//BUG_ON(sched_get_curr() != tcb);//
#endif
	//printk("sys_forkret %d", sched_get_curr() != tcb);
	BUG_ON(current->state != THREAD_RUNNING);
	idg_new_entry(&tcb->ide, RTYPE_THREAD);
	BUG_ON(tcb->ide.id == 0);
	return tcb->ide.id;
}
#endif

int sys_pause(void)
{
	//printk("!!%p", kErnelExiter);BUG();
	sched_next();
	return 0;
}
