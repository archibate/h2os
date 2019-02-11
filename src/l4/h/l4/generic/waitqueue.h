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
	struct ktcb *r;
	if (hlist_empty(&wq->h))
		return NULL;
	r = hlist_entry(wq->h.first, struct ktcb, hlist);
	hlist_del(wq->h.first);
	wq->h.first = wq->h.first->next;
	return r;
}
