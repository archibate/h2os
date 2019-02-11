#pragma once

#include <l4/lib/list.h>

typedef struct wait_queue_head
{
	struct hlist_head h;
}
wait_queue_head_t;

