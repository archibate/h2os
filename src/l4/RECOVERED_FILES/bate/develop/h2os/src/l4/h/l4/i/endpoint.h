#pragma once

#include <l4/types.h>
#include <l4/kobject.h>
#include <l4/waitqueue.h>

typedef struct endpoint
{
	struct kobject ko;
	wait_queue_head_t calling, waiting;
}
endpoint_t;

void epCall(endpoint_t *ep, struct tcb *caller, bool nbsend, bool recv);
void epWait(endpoint_t *ep, struct tcb *waiter);
