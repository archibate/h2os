#pragma once

#include <l4/machine/mmu/types.h>
#include <l4/machine/mmu/page.h>

struct pgdir
{
	pde_t pd[PgdirSize];
}
_PACKED _ALIGNED(PgdirSize * EntrySize);
