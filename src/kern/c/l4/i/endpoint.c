#include <l4/endpoint.h>
#include <l4/sched.h>
#include <memory.h>
#include <assert.h>

static void copyData(tcb_t *recver, tcb_t *sender)
{
	memcpy(recver->extraBuf, sender->extraBuf, sizeof(recver->extraBuf));
}

void epCall(endpoint_t *ep, tcb_t *caller, bool block, bool recv)
{
	assert(caller->state == TCB_Running);
	tcb_t *waiter = wq_pop(&ep->waiting);
	if (waiter) {
		assert(waiter->state == TCB_Waiting);
		copyData(waiter, caller);
		waiter->state = TCB_Running;
		schedActive(waiter);
		if (recv) {
			caller->state = TCB_OnRecv;
			schedSuspend(caller);
			//waiter->replySlot = makeThreadReplyEndpointCap(caller);
		}
	} else if (block) {
		caller->state = recv ? TCB_OnCall : TCB_OnRecv;
		schedSuspend(caller);
		wq_add(&waiter->hlist, &ep->calling);
	}
}

void epWait(endpoint_t *ep, tcb_t *waiter)
{
	assert(waiter->state == TCB_Running);
	tcb_t *caller = wq_pop(&ep->calling);
	if (caller) {
		copyData(waiter, caller);
		switch (caller->state) {
		case TCB_OnCall:
			caller->state = TCB_OnRecv;
			break;
		case TCB_OnSend:
			caller->state = TCB_Running;
			schedActive(caller);
			break;
		default:
			assert(0);
		}
	} else {
		waiter->state = TCB_Waiting;
		schedSuspend(waiter);
		wq_add(&waiter->hlist, &ep->waiting);
	}
}
