#pragma once

#include <l4/object/iframe.h>
#include <l4/object/seframe.h>
#include <l4/machine/mmu/page.h>

struct utcb
{
	struct iframe iframe;
	struct seframe seframe;
}
_PACKED _ALIGNED(PageSize);
