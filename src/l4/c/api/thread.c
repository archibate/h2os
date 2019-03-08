#include <l4/api/thread.h>
#include <l4/generic/thread.h>
#include <l4/generic/idget.h>
#include <l4/enum/thread-states.h>
#include <l4/enum/thread-registers.h>
#include <l4/enum/errno.h>
#include <l4/misc/printk.h>

int sys_thread_suspend(l4id_t tid)
{
	struct ktcb *tcb = id_get_thread(tid);
	if (!tcb) return -ESRCH;

	if (tcb->state != THREAD_RUNNING)
		return -EAGAIN;

	tcb->state = THREAD_SUSPEND;
	thread_suspend(tcb);
	return 0;
}

int sys_thread_check(l4id_t tid)
{
	struct ktcb *tcb = id_get_thread(tid);
	if (!tcb) return -ESRCH;

	if (tcb->state != THREAD_SUSPEND)
		return -EAGAIN;

	return 0;
}

int sys_thread_set_register(l4id_t tid, unsigned int reg, word_t value)
{
	struct ktcb *tcb = id_get_thread(tid);
	if (!tcb) return -ESRCH;

	if (tcb->state != THREAD_SUSPEND)
		return -EAGAIN;

	switch (reg) {
	case THREAD_REG_PC:
		tcb->context.pc = value;
		return 0;
	case THREAD_REG_SP:
		tcb->context.sp = value;
		return 0;
	default:
		return -EINVAL;
	}
}

int sys_thread_active(l4id_t tid)
{
	struct ktcb *tcb = id_get_thread(tid);
	if (!tcb) return -ESRCH;

	if (tcb->state != THREAD_SUSPEND)
		return -EAGAIN;

	tcb->state = THREAD_RUNNING;
	thread_active(tcb);
	return 0;
}

int sys_thread_set_priority(l4id_t tid, unsigned char priority)
{
	struct ktcb *tcb = id_get_thread(tid);
	if (!tcb) return -ESRCH;

	thread_set_priority(tcb, priority);
	return 0;
}

int sys_thread_get_priority(l4id_t tid)
{
	struct ktcb *tcb = id_get_thread(tid);
	if (!tcb) return -ESRCH;

	return tcb->priority;
}

int sys_thread_get_state(l4id_t tid)
{
	struct ktcb *tcb = id_get_thread(tid);
	if (!tcb) return -ESRCH;

	return tcb->state;
}
