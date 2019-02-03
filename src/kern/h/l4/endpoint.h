#pragma once

#include <l4/thread.h>
#include <l4/queue.h>

typedef struct Endpoint
{
	queue_t qCaller;
	queue_t qWaiter;
}
endpoint_t;
