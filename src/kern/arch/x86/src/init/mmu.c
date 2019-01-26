#include <k/kbase.h>
#include <asm/mmu/mmu.h>
#include <asm/mmu/pte.h>
#include <asm/mmu/page.h>

#define kern_pd   ((pde_t*)0x400000)
#define kern_ptes ((pte_t*)0x401000)

void init_mmu(void)
{
	pa_t pa;
	for (pa = 0; pa < KernPhysEnd; pa += PageSize)
		kern_ptes[PageNum(pa)] = Pte(pa, PtePerm_KernRW);

	for (pa = 0; pa < KernPhysEnd; pa += BigPageSize)
		kern_pd[PageNum(pa)] = PdePgtab((va_t)&kern_ptes[PageNum(pa)]);

	mmu_setPgdirPaddr((pa_t)kern_pd);
	mmu_enablePaging();
}
