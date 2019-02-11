#include <l4/generic/endpoint.h>
#include <l4/lib/bug.h>
//#include <memory.h>

static void copy_data(struct ktcb *recver, struct ktcb *sender)
{
	//memcpy(recver->extraBuf, sender->extraBuf, sizeof(recver->extraBuf));
}

void endpoint_call(struct endpoint *ep, struct ktcb *caller, bool block, bool recv)
{
	BUG_ON(caller->state != TCB_Running);
	struct ktcb *waiter = wq_pop(&ep->waiting);
	if (waiter) {
		BUG_ON(waiter->state != TCB_Waiting);
		copy_data(waiter, caller);
		waiter->state = TCB_Running;
		thread_active(waiter);
		if (recv) {
			caller->state = TCB_OnRecv;
			thread_suspend(caller);
			//waiter->replySlot = makeThreadReplyEndpointCap(caller);
		}
	} else if (block) {
		caller->state = recv ? TCB_OnCall : TCB_OnRecv;
		thread_suspend(caller);
		wq_add(&waiter->hlist, &ep->calling);
	}
}

void endpoint_wait(struct endpoint *ep, struct ktcb *waiter)
{
	BUG_ON(waiter->state != TCB_Running);
	struct ktcb *caller = wq_pop(&ep->calling);
	if (caller) {
		copy_data(waiter, caller);
		switch (caller->state) {
		case TCB_OnCall:
			caller->state = TCB_OnRecv;
			break;
		case TCB_OnSend:
			caller->state = TCB_Running;
			thread_active(caller);
			break;
		default:
			BUG();
		}
	} else {
		waiter->state = TCB_Waiting;
		thread_suspend(waiter);
		wq_add(&waiter->hlist, &ep->waiting);
	}
}
