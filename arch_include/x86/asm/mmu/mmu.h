#pragma once

#include <x86/cregs.h>
#include "page.h"

#define mmu_setPgdirPaddr(ppd) setcr3((pa_t)(ppd))
#define mmu_getPgdirPaddr(ppd) PageDown(getcr3())
#define mmu_enablePaging()     setcr0( CR0_PG | getcr0())
#define mmu_disablePaging()    setcr0(~CR0_PG & getcr0())
