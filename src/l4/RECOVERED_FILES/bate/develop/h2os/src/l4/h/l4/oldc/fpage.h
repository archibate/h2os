#pragma once

#include <l4/types.h>

typedef struct FPage
{
	bits_t sizeBits;
	word_t physAddr;
}
fpage_t;
