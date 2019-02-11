#pragma once

#include <l4/lib/list.h>
#include <l4/generic/tcb.h>

typedef struct wait_queue_head
{
	struct hlist_head h;
}
wait_queue_head_t;

static void wq_init(wait_queue_head_t *wq)
{
	hlist_head_init(&wq->h);
}

static void wq_add(wait_queue_head_t *wq, struct ktcb *tcb)
{
	hlist_add_head(&tcb->hlist, &wq->h);
}

static struct ktcb *wq_pop(wait_queue_head_t *wq)
{
	if (hlist_empty(&wq->h))
		return 0;
	hlist_del(wq->h.first);
	return hlist_entry(wq->h.first, struct ktcb, hlist);
}
