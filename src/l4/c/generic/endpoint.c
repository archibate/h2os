#include <l4/generic/endpoint.h>
#include <l4/generic/waitqueue.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/thread.h>
#include <l4/misc/panic.h>//
#include <l4/misc/bug.h>

struct ktcb *endpoint_call(struct endpoint *ep, struct ktcb *caller, bool block, bool recv)
{
	BUG_ON(caller->state != THREAD_RUNNING);
	struct ktcb *waiter = wq_pop(&ep->waiting);
	if (waiter) {
		BUG_ON(waiter->state != THREAD_WAITING);
		waiter->state = THREAD_RUNNING;
		thread_active(waiter);
		if (recv) {
			caller->state = THREAD_ONRECV;
			thread_suspend(caller);
		}
		waiter->replying = recv ? caller : NULL;
		return waiter;

	} else if (block) {
		caller->state = recv ? THREAD_ONCALL : THREAD_ONSEND;
		thread_suspend(caller);
		wq_add(&ep->calling, caller);
		return NULL;
	} else {
		return NULL;
	}
}

struct ktcb *endpoint_wait(struct endpoint *ep, struct ktcb *waiter)
{
	BUG_ON(waiter->state != THREAD_RUNNING);
	struct ktcb *caller = wq_pop(&ep->calling);
	//printk("wait, c:w=%p:%p", caller, waiter);
	if (caller) {
		switch (caller->state) {
		case THREAD_ONCALL:
			caller->state = THREAD_ONRECV;
			waiter->replying = caller;
			break;
		case THREAD_ONSEND:
			caller->state = THREAD_RUNNING;
			thread_active(caller);
			waiter->replying = NULL;
			break;
		default:
			BUG();
		}
		return caller;
	} else {
		waiter->state = THREAD_WAITING;
		BUG_ON(waiter->list.next == NULL);
		thread_suspend(waiter);
		wq_add(&ep->waiting, waiter);
		return NULL;
	}
}

struct ktcb *endpoint_reply(struct ktcb *waiter)
{
	struct ktcb *caller = waiter->replying;
	waiter->replying = NULL;
	if (caller) {
		//printk("!!rep)");
		BUG_ON(caller->state != THREAD_ONRECV);
		caller->state = THREAD_RUNNING;
		thread_active(caller);
	}
	return caller;
}
