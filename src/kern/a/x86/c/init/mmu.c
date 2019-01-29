#include <k/kbase.h>
#include <k/bootml.h>
#include <mmu/mmu.h>
#include <mmu/pte.h>
#include <mmu/page.h>

void init_mmu(void)
{
	pa_t pa;
	for (pa = 0; pa < KernPhysEnd; pa += PageSize)
		kern_ptes[PageNum(pa)] = Pte(pa, PtePerm_KernRW);
	kern_ptes[0] = 0;

	for (pa = 0; pa < KernPhysEnd; pa += SectionSize)
		kern_pd[PdeIndex(pa)] = PdePgtab((va_t)&kern_ptes[PageNum(pa)]);

	mmu_setPgdirPaddr((pa_t)kern_pd, 0);
	mmu_enablePaging();
}
