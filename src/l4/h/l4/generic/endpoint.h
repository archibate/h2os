#pragma once

#include <l4/object/waitqueue.h>

struct endpoint
{
	wait_queue_head_t calling, waiting;
};

struct ktcb;

void endpoint_call(struct endpoint *ep, struct ktcb *caller, bool block, bool recv);
void endpoint_wait(struct endpoint *ep, struct ktcb *waiter);
