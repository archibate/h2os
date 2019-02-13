#include <l4/api/sched.h>
#include <l4/generic/sched.h>
#include <l4/generic/thread.h>
#include <l4/enum/thread-states.h>
#include <l4/misc/bug.h>

l4id_t sys_getpid(void)
{
	return current->ide.id;
}

int sys_exit(void)
{
	current->state = THREAD_SUSPEND;
	thread_suspend(current);
	thread_delete(current);
	BUG_ON(sched_get_curr() == current);
	return 0;
}

int sys_sched_next(void)
{
	sched_next();
	return 0;
}
