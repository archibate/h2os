#include <l4/a/fpgmapwin.h>
#include <l4/a/mmapwin.h>
#include <mmu/pte.h>
#include <mmu/mmu.h>
#include <k/bootml.h>
#include <assert.h>
#include <bittools.h>

static pte_t pgd_getmap(pde_t *pgd, va_t va)
{
	pde_t pde = pgd[PdeIndex(va)];
	if (!PdeIsValid(pde))
		return 0;
	pa_t ptpa = PdePgtabAddr(pde);
	myWindowPgtab[0] = Pte(ptpa, PtePerm_KernRW);
	mmu_invalidatePage(myWindowAddr);
	pte_t *pt = (pte_t*)myWindowAddr;
	return pt[PteIndex(va)];
}

void *mmapwin(mem_t *mem, word_t addr, int rw)
{
	pte_t pte = pgd_getmap(mem->pgd, addr);
	if (!PteIsValid(pte))
		return 0;
	if (PtePerm(pte) != (rw ? PtePerm_UserRW : PtePerm_UserRO))
		return 0;
	pte = Pte(PteAddr(pte), rw ? PtePerm_KernRW : PtePerm_KernRO);
	myWindowPgtab[0] = pte;
	return (void*)myWindowAddr;
}

void *fpgmapwin(fpage_t *fpage, word_t off, int rw)
{
	assert(fpage->sizeBits == PageBits);
	assert(off < SizeOfBits(fpage->sizeBits));
	pte_t pte = Pte(fpage->physAddr, rw ? PtePerm_KernRW : PtePerm_KernRO);
	myWindowPgtab[0] = pte;
	return (void*)(myWindowAddr + off);
}
