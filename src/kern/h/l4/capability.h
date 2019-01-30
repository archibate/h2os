#pragma once

#include <l4/types.h>
#include <l4/fpage.h>
#include <l4/segment.h>

typedef struct capability
{
	word_t c_type;
	union {
		struct {
			segment_t seg;
			void *c_ptr;
		};
		struct {
			word_t c_labUsed;
			fpage_t fpage;
		};
	};
}
cap_t;
#define c_base		seg.base
#define c_limit		seg.limit
#define c_sizeBits	fpage.sizeBits
#define c_physAddr	fpage.physAddr

#include <stassert.h>

static void __stassertF1212312(void)
{
	static_assert(2 * sizeof(word_t) == sizeof(segment_t));
	static_assert(2 * sizeof(word_t) == sizeof(fpage_t));
}
