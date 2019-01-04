#pragma once

#include <x86/cregs.h>
#include <asm/page.h>
#include <inttypes.h>

#define mmu_set_ppd(ppd) setcr3((ulong)(ppd))
#define mmu_get_ppd(ppd) PGDN(getcr3())
