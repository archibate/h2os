#include <l4/generic/endpoint.h>
#include <l4/generic/waitqueue.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/thread.h>
#include <l4/misc/panic.h>//
#include <l4/misc/bug.h>
#include <memory.h>

void endpoint_init(struct endpoint *ep)
{
	memset(ep, 0, sizeof(*ep));
}

void endpoint_revoke(struct endpoint *ep)
{
}

void endpoint_delete(struct endpoint *ep)
{
	endpoint_revoke(ep);
	hlist_del(&ep->ide.hlist);
}

static void copy_data(struct ktcb *recver, struct ktcb *sender)
{
	//memcpy(recver->extraBuf, sender->extraBuf, sizeof(recver->extraBuf));
}

void endpoint_call(struct endpoint *ep, struct ktcb *caller, bool block, bool recv)
{
	BUG_ON(caller->state != THREAD_RUNNING);
	struct ktcb *waiter = wq_pop(&ep->waiting);
	if (waiter) {
		BUG_ON(waiter->state != THREAD_WAITING);
		copy_data(waiter, caller);
		waiter->state = THREAD_RUNNING;
		thread_active(waiter);
		if (recv) {
			caller->state = THREAD_ONRECV;
			thread_suspend(caller);
			//waiter->replySlot = makeThreadReplyEndpointCap(caller);
		}
	} else if (block) {
		caller->state = recv ? THREAD_ONCALL : THREAD_ONSEND;
		thread_suspend(caller);
		wq_add(&ep->calling, waiter);
	}
}

void endpoint_wait(struct endpoint *ep, struct ktcb *waiter)
{
	BUG_ON(waiter->state != THREAD_RUNNING);
	struct ktcb *caller = wq_pop(&ep->calling);
	//printk("wait, c:w=%p:%p", caller, waiter);
	if (caller) {
		copy_data(waiter, caller);
		switch (caller->state) {
		case THREAD_ONCALL:
			caller->state = THREAD_ONRECV;
			break;
		case THREAD_ONSEND:
			caller->state = THREAD_RUNNING;
			thread_active(caller);
			break;
		default:
			BUG();
		}
	} else {
		waiter->state = THREAD_WAITING;
		BUG_ON(waiter->list.next == NULL);
		thread_suspend(waiter);
		wq_add(&ep->waiting, waiter);
	}
}
