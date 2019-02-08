#pragma once

#include <l4/thread.h>
#include <list.h>

typedef struct Endpoint
{
	struct hlist_head qCaller;
	struct hlist_head qWaiter;
}
endpoint_t;

void epCall(endpoint_t *ep, tcb_t *caller, bool nbsend, bool recv);
void epWait(endpoint_t *ep, tcb_t *waiter);
