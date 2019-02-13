#pragma once

#include <l4/object/waitqueue.h>
#include <l4/object/identry.h>

struct async_ep
{
	struct ids_entry ide;
	wait_queue_head_t listening;
	unsigned int count;
};
