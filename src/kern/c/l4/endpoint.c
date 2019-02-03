#include <l4/endpoint.h>
#include <l4/sched.h>
#include <memory.h>
#include <assert.h>

void copyData(tcb_t *recver, tcb_t *sender)
{
	memcpy(recver->extraBuf, sender->extraBuf, sizeof(recver->extraBuf));
}

void epCall(endpoint_t *ep, tcb_t *caller, bool nbsend, bool recv)
{
	assert(caller->state == TCB_Running);
	tcb_t *waiter = queuePopLast(&ep->qWaiter, tcb_t);
	if (waiter) {
		assert(waiter->state == TCB_Waiting);
		copyData(waiter, caller);
		waiter->state = TCB_Running;
		schedSetActive(waiter);
		if (recv) {
			caller->state = TCB_OnRecv;
			schedSetInactive(caller);
			//waiter->replySlot = makeThreadReplyEndpointCap(caller);
		}
	} else {
		caller->state = recv ? TCB_OnCall : TCB_OnRecv;
		schedSetInactive(caller);
		queueAddFirst(&ep->qCaller, caller);
	}
}

void epWait(endpoint_t *ep, tcb_t *waiter)
{
	assert(waiter->state == TCB_Running);
	tcb_t *caller = queuePopLast(&ep->qCaller, tcb_t);
	if (caller) {
		copyData(waiter, caller);
		switch (caller->state) {
		case TCB_OnCall:
			caller->state = TCB_OnRecv;
			break;
		case TCB_OnSend:
			caller->state = TCB_Running;
			schedSetActive(caller);
			break;
		default:
			assert(0);
		}
	} else {
		waiter->state = TCB_Waiting;
		schedSetInactive(waiter);
		queueAddFirst(&ep->qWaiter, waiter);
	}
}
