#pragma once

#include <l4/types.h>

typedef struct FPage
{
	word_t objType;
	bits_t fpSizeBits;
	word_t labUsed;
}
fpage_t;
