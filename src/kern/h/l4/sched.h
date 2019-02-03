#pragma once

#include <l4/thread.h>
#include <l4/queue.h>

extern queue_t running;
#define schedGetCurr()		queueFirst(&running, tcb_t)
#define currTcb			schedGetCurr()

#define schedInit()		queueInit(&running)
#define schedNext()		queueNext(&running)
#define schedSetActive(x)	queueAddFirst(&running, x)
#define schedSetInactive(x)	queueRemove(x)
