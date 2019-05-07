#include <l4/api/sched.h>
#include <l4/generic/sched.h>
#include <l4/generic/thread.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/endpoint.h>
#include <l4/misc/bug.h>

l4id_t sys_getpid(void)
{
	return current->ide.id;
}

int sys_exit(void)
{
	while (NULL != endpoint_call(&current->ep_death, current, false, false));
	current->state = THREAD_SUSPEND;
	thread_suspend(current);
	thread_delete(current);
	BUG_ON(sched_get_curr() == current);
	return 0;
}

int sys_wait_first(void)
{
	if (hlist_empty(&current->children))
		return 0;
	struct ktcb *tcb = hlist_entry(__hlist_pop(&current->children), struct ktcb, hlist_child);
	endpoint_wait(&tcb->ep_death, current, true);
	return hlist_empty(&current->children) ? 1 : 2;
}

int sys_sched_next(void)
{
	sched_next();
	return 0;
}
