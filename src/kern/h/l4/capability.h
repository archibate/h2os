#pragma once

#include <l4/types.h>

typedef struct capability
{
	word_t objType; // as described in <l4/objects.h>
	word_t base, limit;
	void *object;
}
cap_t;
