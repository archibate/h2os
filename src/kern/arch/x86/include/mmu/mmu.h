#pragma once

#include <x86/cregs.h>
#include <x86/invlpg.h>
#include "types.h"
#include "page.h"

#define mmu_invalidatePage(va)       invlpg((void*)(va))
#define mmu_setPgdirPaddr(pd, flgs)  setcr3(PageDown(pd) | (flgs))
#define mmu_getPgdirPaddr(pd)        PageDown(getcr3())
#define mmu_enablePaging()           setcr0( CR0_PG | getcr0())
#define mmu_disablePaging()          setcr0(~CR0_PG & getcr0())

#define MMU_CacheDisable             CR3_PCD
#define MMU_WriteThrough             CR3_PWT
