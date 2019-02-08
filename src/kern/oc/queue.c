#include <l4/queue.h>
#include <k/printk.h>
#include <k/panic.h>
#include <assert.h>

void queueInit(queue_t *q)
{
	q->next = q->prev = q;
	dprintk("qInit:%p!!", q);
}

void queueNext(queue_t *q)
{
	dprintk("qNext:%p(%p:%p)!!", q, q->next, q->next->next);
	node_t *n = q->next->next;
	node_t *p = q->prev->next;
	assert(q->next != n);
	assert(q->prev != p);
	q->next = n;
	q->prev = p;
}

void queuePrev(queue_t *q)
{
	dprintk("qPrev:%p!!", q);
	node_t *n = q->next->prev;
	node_t *p = q->prev->prev;
	q->next = n;
	q->prev = p;
}

void _queueAddFirst(queue_t *q, node_t *node)
{
	dprintk("qAddFirst:%p:%p!!", q, node);
	node->next = q->next;
	node->prev = q;
	q->next = node;
	if (q->prev == q)
		q->prev = node;
	dprintk("%p:%p!!", q, q->next);
}

void _queueAddLast(queue_t *q, node_t *node)
{
	dprintk("qAddLast:%p:%p!!", q, node);
	node->prev = q->prev;
	node->next = q;
	q->prev = node;
	if (q->next == q)
		q->next = node;
}

node_t *_queuePopFirst(queue_t *q)
{
	dprintk("qPopFirst:%p!!", q);
	return queueEmpty(q) ? 0 : _queueRemove(q->next);
}

node_t *_queuePopLast(queue_t *q)
{
	dprintk("qPopLast:%p!!", q);
	return queueEmpty(q) ? 0 : _queueRemove(q->prev);
}

node_t *_queueRemove(node_t *node)
{
	dprintk("qRemove:%p!!", node);
	node_t *prev = node->prev;
	node_t *next = node->next;
	prev->next = next;
	next->prev = prev;
	return node;
}
