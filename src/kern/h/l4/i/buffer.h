#pragma once

#include <l4/types.h>
#include <l4/kobject.h>

typedef struct CBuffer {
	word_t *objptr;
	struct kobject ko;
	word_t limit;
	word_t water;
} CBuffer_t;
