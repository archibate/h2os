#include <l4/api/thread.h>
#include <l4/generic/thread.h>
#include <l4/generic/cap-convert.h>
#include <l4/enum/cap-actions.h>
#include <l4/enum/errno.h>

int cap_thread_action(struct capability *cap, unsigned int action, unsigned long argument)
{
	struct ktcb *tcb = cap_get_thread(cap);

	switch (action)
	{
	case CAP_THREAD_SUSPEND:
		thread_suspend(tcb);
		return 0;

	case CAP_THREAD_ACTIVE:
		thread_active(tcb);
		return 0;

	case CAP_THREAD_SET_PRIORITY:
		thread_set_priority(tcb, argument);
		return 0;

	default:
		return -ENOSYS;
	}
}
