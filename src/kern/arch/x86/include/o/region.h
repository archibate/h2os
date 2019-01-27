#pragma once

#include <mmu/types.h>

typedef struct Region
{
	va_t va0, va1;
}
Region_t;

struct Frames;
void Region_Map(Region_t const *reg, struct Frames *fsMap, struct Frames *fsPgtab);
