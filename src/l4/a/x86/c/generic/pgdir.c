#include <l4/generic/pgdir.h>
#include <l4/machine/mmu/types.h>
#include <l4/machine/mmu/page.h>
#include <l4/machine/mmu/pte.h>
#include <l4/machine/mmu/mmu.h>
#include <l4/system/kbase.h>
#include <l4/system/kmm.h>
#include <l4/misc/bug.h>
#include <memory.h>

void pgdir_init(struct pgdir *pgdir)
{
	memcpy(&pgdir->pd, kPgdir, PdeIndex(KernVirtEnd) * EntrySize);
}

void pgdir_switch(struct pgdir *pgdir, struct utcb *utcb_ptr)
{
	pa_t utcb = (pa_t)utcb_ptr;
	pa_t pd = (pa_t)&pgdir->pd;

	BUG_ON(!utcb || PageOffset(utcb));

	kPtes[PageNum(KernUTCBAddr)] = Pte(utcb, PtePerm_KernRW);

	if (mmu_getPgdirPaddr() == pd) {
		mmu_invalidatePage(utcb);

	} else {
		BUG_ON(!pd || PgdirOffset(pd));
		BUG_ON(!pgdir->pd[0]);

		mmu_setPgdirPaddr(pd);
	}
}
