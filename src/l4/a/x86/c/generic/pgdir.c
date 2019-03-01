#include <l4/generic/pgdir.h>
#include <l4/machine/mmu/types.h>
#include <l4/machine/mmu/page.h>
#include <l4/machine/mmu/pte.h>
#include <l4/machine/mmu/mmu.h>
#include <l4/system/kstack.h>
#include <l4/system/kbase.h>
#include <l4/system/kmm.h>
#include <l4/misc/bug.h>
#include <memory.h>

void pgdir_init(struct pgdir *pgdir)
{
	memcpy(&pgdir->pd, kPd, PdeIndex(KernVirtEnd) * EntrySize);
}

void pgdir_switch(struct pgdir *pgdir,
		struct utcb *utcb_ptr,
		struct ipc_buf *ipcbuf_ptr)
{
	pa_t utcb = (pa_t)utcb_ptr;
	pa_t ipcbuf = (pa_t)ipcbuf_ptr;
	pa_t pd = (pa_t)&pgdir->pd;

	BUG_ON(!utcb || PageOffset(utcb));

	kPtes[PageNum(KernUTCBAddr)] = Pte(utcb, PtePerm_KernRW);
	kPtes[PageNum(KernIPCBuffer)] = Pte(ipcbuf, PtePerm_UserRW);

	if (mmu_getPgdirPaddr() == pd) {
		mmu_invaidatePage(KernUTCBAddr);
		mmu_invaidatePage(KernIPCBuffer);

	} else {
		BUG_ON(!pd || PgdirOffset(pd));
		mmu_setPgdirPaddr(pd);
	}
}

void __pgdir_switch_halt_utcb(void)
{
	kPtes[PageNum(KernUTCBAddr)] = Pte(KernUTCBAddr, PtePerm_KernRW);
	mmu_invaidatePage(KernUTCBAddr);
}
