#pragma once

#include <l4/generic/tcb.h>
#include <l4/generic/waitqueue.h>

struct endpoint
{
	wait_queue_head_t waiting, calling;
};

void endpoint_call(struct endpoint *ep, struct ktcb *caller, bool block, bool recv);
void endpoint_wait(struct endpoint *ep, struct ktcb *waiter);
