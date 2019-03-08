#pragma once

#include <l4/object/iframe.h>
#include <l4/object/seframe.h>
#include <l4/machine/mmu/page.h>

struct utcb
{
	void _NORETURN (*exiter)(void);
	struct seframe seframe;
	struct iframe iframe;
	//void _NORETURN (*saved_exiter)(void);
}
_PACKED _ALIGNED(PageSize);
