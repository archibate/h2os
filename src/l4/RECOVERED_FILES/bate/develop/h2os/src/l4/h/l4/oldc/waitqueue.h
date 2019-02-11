#pragma once

#include <l4/i/tcb.h>

typedef struct wait_queue_head
{
	struct hlist_head h;
}
wait_queue_head_t;

static void wq_init(wait_queue_head_t *wq)
{
	hlist_head_init(&wq->h);
}

static void wq_add(wait_queue_head_t *wq, tcb_t *tcb)
{
	hlist_add_head(&tcb->ko.hlist, &wq->h);
}

static tcb_t *wq_pop(wait_queue_head_t *wq)
{
	if (hlist_empty(&wq->h))
		return 0;
	hlist_del(wq->h.first);
	return hlist_entry(wq->h.first, tcb_t, ko.hlist);
}
