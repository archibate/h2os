#pragma once

#include <x86/cregs.h>
#include <x86/invlpg.h>
#include "page.h"

#define mmu_invalidatePage(va) invlpg((void*)(va))
#define mmu_setPgdirPaddr(pd)  setcr3((pa_t)(pd))
#define mmu_getPgdirPaddr(pd)  PageDown(getcr3())
#define mmu_enablePaging()     setcr0( CR0_PG | getcr0())
#define mmu_disablePaging()    setcr0(~CR0_PG & getcr0())
