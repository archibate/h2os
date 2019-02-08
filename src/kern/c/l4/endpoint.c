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
	if (!hlist_empty(&ep->qWaiter)) {
		hlist_del(ep->qWaiter.first);
		tcb_t *waiter = hlist_entry(ep->qWaiter.first, tcb_t, hlist);
		assert(waiter->state == TCB_Waiting);
		copyData(waiter, caller);
		waiter->state = TCB_Running;
		schedSetActive(waiter);
		if (recv) {
			caller->state = TCB_OnRecv;
			schedSetInactive(caller);
			//waiter->replySlot = makeThreadReplyEndpointCap(caller);
		}
	} else if (block) {
		caller->state = recv ? TCB_OnCall : TCB_OnRecv;
		schedSetInactive(caller);
		hlist_add_head(&caller->hlist, &ep->qWaiter);
	}
}

void epWait(endpoint_t *ep, tcb_t *waiter)
{
	assert(waiter->state == TCB_Running);
	if (!hlist_empty(&ep->qCaller)) {
		hlist_del(ep->qCaller.first);
		tcb_t *caller = hlist_entry(ep->qCaller.first, tcb_t, hlist);
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
		hlist_add_head(&waiter->hlist, &ep->qCaller);
	}
}
