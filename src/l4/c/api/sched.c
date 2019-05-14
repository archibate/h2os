#include <l4/api/sched.h>
#include <l4/generic/sched.h>
#include <l4/generic/task-switch.h>
#include <l4/generic/thread.h>
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

int sys_pause(void)
{
	//printk("!!%p", kErnelExiter);BUG();
	sched_next();
	return 0;
}
