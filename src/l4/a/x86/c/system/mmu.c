#include <l4/system/kmm.h>
#include <l4/system/kbase.h>
#include <l4/machine/mmu/types.h>
#include <l4/machine/mmu/page.h>
#include <l4/machine/mmu/mmu.h>
#include <l4/machine/mmu/pte.h>

void init_mmu(void)
{
	pa_t pa;
	for (pa = 0; pa < KernPhysEnd; pa += PageSize)
		kPtes[PageNum(pa)] = Pte(pa, PtePerm_KernRW);
	kPtes[0] = 0;

	for (pa = 0; pa < KernVirtEnd; pa += SectionSize)
		kPd[PdeIndex(pa)] = PdePgtab((va_t)&kPtes[PageNum(pa)]);

	mmu_setPgdirPaddrEx((pa_t)kPd, 0);
	mmu_enablePaging();
}
