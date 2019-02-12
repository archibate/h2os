#pragma once

#include <l4/object/waitqueue.h>
#include <l4/object/thread.h>

void wq_init(wait_queue_head_t *wq)
{
	hlist_head_init(&wq->h);
}

void wq_add(wait_queue_head_t *wq, struct ktcb *tcb)
{
	hlist_add_head(&tcb->hlist, &wq->h);
}

struct ktcb *wq_pop(wait_queue_head_t *wq)
{
	if (hlist_empty(&wq->h))
		return NULL;
	return hlist_entry(__hlist_pop(&wq->h), struct ktcb, hlist);
}
