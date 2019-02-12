#pragma once

#include <l4/object/waitqueue.h>
#include <l4/object/identry.h>

struct endpoint
{
	struct ids_entry ide;
	wait_queue_head_t calling, waiting;
};
