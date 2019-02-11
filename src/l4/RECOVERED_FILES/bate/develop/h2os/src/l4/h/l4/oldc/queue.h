#pragma once

#error

typedef struct list_head queue_t;

#define queueInit(q)        list_init(q)
#define _queueAdd(q)        list_add(q)
#define _queueAddLast(q)    list_add_tail(q)
#define _queueRemove(x)     list_del(x)
#define _queueMove(x,q)     list_move(x,q)
#define _queueMove(x,q)     list_move(x,q)

#define _queueFirst(q)	((q)->next)
#define _queueLast(q)	((q)->next)
#define queueEmpty(q)	((q)->next == (q) && (q) == (q)->prev)

#if 0
void queueInit(queue_t *q);
void queueNext(queue_t *q);
void queuePrev(queue_t *q);
void _queueAddFirst(queue_t *q, node_t *node);
void _queueAddLast(queue_t *q, node_t *node);
node_t *_queuePopFirst(queue_t *q);
node_t *_queuePopLast(queue_t *q);
node_t *_queueRemove(node_t *node);
#endif

#include <stddef.h>
#define N(x)	(&(x)->node)
#define UN(t,n)	(entryof(t,node,n))

#define queueFirst(q,t) (queueEmpty(q)?0:UN(t,_queueFirst(q)))
#define queueLast(q,t) (queueEmpty(q)?0:UN(t,_queueLast(q)))
#define queuePopFirst(q,t) (queueEmpty(q)?0:UN(t,_queueRemove((q)->next)))
#define queuePopLast(q,t) (queueEmpty(q)?0:UN(t,_queueRemove((q)->prev)))
#define queueAddFirst(q,x) _queueAddFirst(q,N(x))
#define queueAddLast(q,x) _queueAddLast(q,N(x))
#define queueRemove(x) _queueRemove(N(x))
#define queueMove(x,q) _queueMove(N(x),q)
