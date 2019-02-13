#include <l4/generic/asyncep.h>
#include <l4/generic/sched.h>
#include <l4/generic/thread.h>
#include <l4/generic/waitqueue.h>
#include <l4/enum/thread-states.h>
#include <memory.h>

void async_init(struct async_ep *aep)
{
	memset(aep, 0, sizeof(*aep));
}

void async_revoke(struct async_ep *aep)
{
	//popoutwqallo
}

void async_delete(struct async_ep *aep)
{
	async_revoke(aep);
}

void async_pulse(struct async_ep *aep)
{
	struct ktcb *listener = wq_pop(&aep->listening);
	if (listener) {
		listener->state = THREAD_RUNNING;
		thread_active(listener);

	} else {
		aep->count++;
	}
}

bool async_poll(struct async_ep *aep)
{
	if (aep->count > 0) {
		aep->count--;
		return true;

	} else {
		return false;
	}
}

void async_listen(struct async_ep *aep, struct ktcb *listener)
{
	if (aep->count > 0) {
		aep->count--;

	} else {
		current->state = THREAD_LISTENING;
		thread_suspend(listener);
		wq_add(&aep->listening, listener);
	}
}
