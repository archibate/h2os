#include <l4/generic/archmappage.h>
#include <l4/generic/allocpage.h>
#include <l4/object/pgdir.h>
//#include <l4/generic/pgdir.h>
#include <l4/machine/mmu/types.h>
#include <l4/machine/mmu/mmu.h>
#include <l4/machine/mmu/page.h>
#include <l4/machine/mmu/pte.h>

static int pgdir_map_entry(pde_t *src_pd, pde_t *dst_pd,
		word_t src_addr, word_t dst_addr, int succ)
{
	bool grant = succ & PG_P;
	uint permask = succ & PG_W;

	pte_t *pt;
	pde_t pde;
	pte_t pte;

	pde = src_pd[PdeIndex(src_addr)];
	if (!PdeIsValid(pde))
		return -1;
	pt = (pte_t *)PdePgtabAddr(pde);
	pte = pt[PteIndex(src_addr)];
	if (!PteIsValid(pde))
		return -1;
	if (grant)
		pt[PteIndex(src_addr)] = 0;

	pde = dst_pd[PdeIndex(dst_addr)];
	if (!PdeIsValid(pde)) {
		pt = alloc_page();
		dst_pd[PdeIndex(dst_addr)] = PdePgtab((word_t)pt);
	} else {
		pt = (pte_t *)PdePgtabAddr(pde);
	}
	pt[PteIndex(dst_addr)] = pte;// & permask;
	return 0;
}

int arch_mm_map_page(struct mm *mm, word_t addr, int succ, word_t page)
{
	pde_t *src_pd = (pde_t *) mmu_getPgdirPaddr();
	pde_t *dst_pd = mm->pgdir->pd;
	word_t src_addr = page;
	word_t dst_addr = addr;

	return pgdir_map_entry(src_pd, dst_pd, src_addr, dst_addr, succ);
}
