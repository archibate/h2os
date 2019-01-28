#include <o/region.h>
#include <o/frames.h>
#include <mmu/types.h>
#include <mmu/mmu.h>
#include <mmu/page.h>
#include <mmu/pte.h>
#include <assert.h>

static pte_t *touch_pdi(Frames_t *fs, pde_t *pd, uint pdi)
{
	pte_t *pt;
	if (!PdeIsValid(pd[pdi])) {
		pt = (pte_t*)PteAddr(_Frames_GetFramePte(fs));
		pd[pdi] = PdePgtab((va_t)pt);
	} else {
		pt = (pte_t*)PdePgtabAddr(pd[pdi]);
	}
	return pt;
}

void Region_Map(Region_t const *r, Frames_t *fsMap, Frames_t *fsPgtab)
{
	assert(PageOffset(r->va0) == 0);
	assert(PageOffset(r->va1) == 0);

	pde_t *pd = (pde_t*)mmu_getPgdirPaddr();
	pte_t *pt = touch_pdi(fsPgtab, pd, PdeIndex(r->va0));
	va_t va = r->va0;

	size_t secoff = SectionOffset(va);

	for (; va < r->va1; va += PageSize, secoff += PageSize)
	{
		if (secoff >= SectionSize) {
			pt = touch_pdi(fsPgtab, pd, PdeIndex(va));
			secoff = 0;
		}
		pt[PteIndex(va)] = _Frames_GetFramePte(fsMap);
		mmu_invalidatePage(va);
	}
}
