#pragma once

#include <l4/types.h>
#include <l4/segment.h>

typedef struct capability
{
	byte_t c_type;
	union {
		struct {
			segment_t seg;
			word_t c_water;
		};
		void *c_objptr;
	}
}
cap_t;
#define c_base		seg.base
#define c_limit		seg.limit

#include <stassert.h>

static void __stassertF1212312(void)
{
	static_assert(2 * sizeof(word_t) == sizeof(segment_t));
}
