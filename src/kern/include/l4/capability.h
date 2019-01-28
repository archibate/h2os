#pragma once

#include <l4/types.h>
#include <l4/objects.h>
#include <ccutils.h>

#define CapRawWords 3

typedef struct capability
{
	word_t objType; // as described in <l4/objects.h>
	word_t raw[CapRawWords]; // may contains an object or a pointer
}
_PACKED cap_t;

#define getCapType(cap)      ((cap)->objType)
#define getCapPtr_RAWED(cap) ((void*)&(cap)->raw)
#define getCapPtr_PTRED(cap) ((void*)(cap)->raw[0])
#define setCapPtr_PTRED(cap, ptr) ((void)((cap)->raw[0] = (ptr)))

static void deleteCap(cap_t *cap)
{
	cap->objType = 0;
}

static void moveCap(cap_t *dest, cap_t *src)
{
	deleteCap(dest);
	*dest = *src;
	src->objType = 0;
}

static void swapCap(cap_t *dest, cap_t *src)
{
	cap_t tmp = *dest;
	*dest = *src;
	*src = tmp;
}
