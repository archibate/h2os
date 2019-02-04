#include <o/region.h>
#include <o/frames.h>

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
