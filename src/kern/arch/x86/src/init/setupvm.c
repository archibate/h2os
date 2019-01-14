#include <hydro/kbase.h>
#include <asm/mmu.h>

//extern pde_t kern_pd  [PDXSIZE] _ALIGNED(PDXSIZE * sizeof(pde_t));
//extern pte_t kern_ptes[PTXSIZE] _ALIGNED(PTXSIZE * sizeof(pte_t));
// for now:
#define kern_pd   ((pde_t*)0x400000)
#define kern_ptes ((pte_t*)0x401000)

void setup_boot_vm(void)
{
	pa_t pa;
	for (pa = 0; pa < PHYSTOP; pa += PGSIZE)
		kern_ptes[PGNUM(pa)] = PTE(pa, PTE_P | PTE_K | PTE_W);

	mmu_set_ppd((pa_t)kern_pd);
}
