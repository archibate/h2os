#include <l4/types.h>
#include <k/kstack.h>
#include <k/bootml.h>
#include <mmu/page.h>
#include <mmu/mmu.h>
#include <mmu/pte.h>
#include <memory.h>
#include <k/printk.h>
#include <assert.h>
#include <stassert.h>

void Arch_InitPgdir(pde_t *pd)
{
	const size_t len = PdeIndex(KernVirtEnd) * sizeof(pde_t);
	memcpy(pd, kPgdir, len);
}

void Arch_switchPgdirAndUTCB(pde_t *newPd, pa_t utcb)
{
#if 0
	static_assert(!SectionOffset(KernVirtEnd));
	static_assert(!SectionOffset(KernVirtBegin));
	const size_t len = PdeIndex(KernVirtEnd - KernVirtBegin) * sizeof(pde_t);
	const size_t off = PdeIndex(KernVirtBegin) * sizeof(pde_t);
#endif
	//dprintk("Arch_switchPgdirAndUTCB(%p, %p, %p)", kPgdir, newPd, utcb);
	assert(utcb);
	assert(!PageOffset(utcb));
	kern_ptes[PageNum(KernUTCBAddr)] = Pte(utcb, PtePerm_KernRW);
	if (PteAddr(kern_ptes[PageNum(KernPgdirAddr)]) == (pa_t)newPd) {
		mmu_invalidatePage(utcb);
	} else {
		assert(kPgdir && newPd);
		assert(!PageOffset((va_t)kPgdir));
		assert(!PgdirOffset((va_t)newPd));
		assert(newPd[0]);
		mmu_setPgdirPaddr((pa_t)newPd);
	}
}
