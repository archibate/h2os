#pragma once

#include <l4/machine/asm/cregs.h>
#include <l4/machine/asm/invlpg.h>
#include "types.h"
#include "page.h"

#define mmu_invaidatePage(va)       invlpg((void*)(va))
#define mmu_invaidateAll()          setcr3(getcr3())
#define mmu_setPgdirPaddrEx(pd, flg) setcr3(PageDown(pd) | (flg))
#define mmu_setPgdirPaddr(pd)        setcr3(PageDown(pd))
#define mmu_getPgdirPaddr(pd)        PageDown(getcr3())
#define mmu_enablePaging()           setcr0( CR0_PG | getcr0())
#define mmu_disablePaging()          setcr0(~CR0_PG & getcr0())

#define MMU_CacheDisable             CR3_PCD
#define MMU_WriteThrough             CR3_PWT
