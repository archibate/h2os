#include <l4/types.h>
#include <k/kstack.h>
#include <l4/a/mswitch.h>
#include <k/bootml.h>
#include <mmu/page.h>
#include <mmu/mmu.h>
#include <mmu/pte.h>
#include <memory.h>
#include <assert.h>

void Arch_InitPgdir(pde_t *pd)
{
	const size_t len = PdeIndex(KernPhysEnd) * sizeof(pde_t);
	memcpy(pd, kern_pd, len);
}

void Arch_switchPgdirAndUTCB(pde_t *oldPd, pde_t *newPd, pa_t utcb)
{
	const size_t len = PdeIndex(KernVirtEnd) * sizeof(pde_t);
	const size_t off = PdeIndex(KernVirtBegin) * sizeof(pde_t);
	if (oldPd == newPd) {
		assert(utcb);
		assert(!PageOffset(utcb));
		kern_ptes[PageNum(KernUTCBAddr)] = Pte(utcb, PtePerm_KernRW);
		mmu_invalidatePage(utcb);
	} else {
		assert(utcb);
		assert(!PageOffset(utcb));
		assert(oldPd && newPd);
		assert(!PageOffset((va_t)oldPd));
		assert(!PgdirOffset((va_t)newPd));
		assert(newPd[0]);
		kern_ptes[PageNum(KernUTCBAddr)] = Pte(utcb, PtePerm_KernRW);
		memcpy(newPd + off, oldPd + off, len);
		mmu_setPgdirPaddr((pa_t)newPd);
	}
	extern void se_iframe_exiter(void);
	kSEFrame[-1] = (word_t)se_iframe_exiter;
}
