#include <l4/api/thread.h>
#include <l4/generic/thread.h>
#include <l4/generic/idget.h>
#include <l4/enum/thread-states.h>
#include <l4/enum/errno.h>

int sys_thread_suspend(l4id_t tid)
{
	struct ktcb *tcb = id_get_thread(tid);

	if (tcb->state != THREAD_RUNNING)
		return -EAGAIN;

	tcb->state = THREAD_SUSPEND;
	thread_suspend(tcb);
	return 0;
}

int sys_thread_active(l4id_t tid)
{
	struct ktcb *tcb = id_get_thread(tid);

	if (tcb->state != THREAD_SUSPEND)
		return -EAGAIN;

	tcb->state = THREAD_RUNNING;
	thread_active(tcb);
	return 0;
}

int sys_thread_set_priority(l4id_t tid, unsigned int priority)
{
	struct ktcb *tcb = id_get_thread(tid);

	thread_set_priority(tcb, priority);
	return 0;
}
