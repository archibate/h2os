#include <l4/a/mkvpage.h>
#include <k/bootml.h>
#include <mmu/types.h>
#include <mmu/page.h>
#include <mmu/pte.h>
#include <k/panic.h>
#include <assert.h>

static word_t vaddr = KernVirtBegin;

void *Arch_makeVirtPage(word_t paddr)
{
	assert(paddr < KernPhysEnd);
	if (vaddr > KernVirtEnd)
		panic("Out of Kernel Virt Map Area");
	kern_ptes[PageNum(vaddr)] = Pte(paddr, PtePerm_KernRW);
	void *v = (void*)vaddr;
	vaddr += PageSize;
	return v;
}
