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
#include <l4/misc/printk.h>//

void pgdir_init(struct pgdir *pgdir)
{
	memcpy(&pgdir->pd, kPd, PdeIndex(KernVirtEnd) * EntrySize);
}

void pgdir_switch(struct pgdir *pgdir, struct ipc_buf *ipcbuf_ptr)
{
	pa_t ipcbuf = (pa_t)ipcbuf_ptr;
	pa_t pd = (pa_t)&pgdir->pd;

	BUG_ON(!ipcbuf || PageOffset(ipcbuf));

	kPtes[PageNum(KernIPCBuffer)] = Pte(ipcbuf, PtePerm_UserRW);

	if (mmu_getPgdirPaddr() == pd) {
		mmu_invaidatePage(KernIPCBuffer);

	} else {
		BUG_ON(!pd || PgdirOffset(pd));
		mmu_setPgdirPaddr(pd);
	}
}

void set_idle_task(void)
{
	extern void _NORETURN idle_exiter(void);
	kErnelExiter = idle_exiter;
}
