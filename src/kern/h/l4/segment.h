#pragma once

#include <l4/types.h>

typedef struct Segment
{
	word_t base, limit;
}
segment_t;
