#include <hydro/kbase.h>
#include <asm/mmu.h>
#include <ccutils.h>

//extern pde_t kern_pd  [PDXSIZE] _ALIGNED(PDXSIZE * sizeof(pde_t));
//extern pte_t kern_ptes[PTXSIZE] _ALIGNED(PTXSIZE * sizeof(pte_t));
// for now:
#define kern_pd   ((pde_t*)0x400000)
#define kern_ptes ((pte_t*)0x401000)

_SECTION(".phys.text") void setup_boot_vm(void)
{
	pa_t pa;
	for (pa = 0; pa < PHYSTOP; pa += PGSIZE)
		kern_ptes[PGNUM(pa)] = PTE(pa, PTE_P | PTE_K | PTE_W);

	for (pa = 0; pa < PHYSTOP; pa += PGSIZE * PTXSIZE) {
		pa_t  pta = KV2P((va_t)&kern_ptes[PGNUM(pa)]);
		pde_t pde = PDE(pta, PDE_P | PDE_K | PDE_W);
		kern_pd[PDX(KP2V(pa))] = pde;
		kern_pd[PDX(pa)] = pde;
	}

	mmu_set_ppd(KV2P((va_t)kern_pd));
}
