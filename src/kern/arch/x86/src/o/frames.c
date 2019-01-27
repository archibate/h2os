#include <o/frames.h>
#include <mmu/types.h>
#include <mmu/page.h>
#include <mmu/pte.h>
#include <k/bootml.h>
#include <memory.h>

static int alloc_page_pa = kern_ptpool_beg;
static pa_t alloc_page(void)
{
	pa_t pa = alloc_page_pa;
	alloc_page_pa += PageSize;
	return pa;
}

pte_t _Frames_GetFramePte(Frames_t *fg)
{
	pa_t pa = alloc_page();
	memset((void*)pa, 0, PageSize);//P2V
	return Pte(pa, fg->ptePerm);
}
