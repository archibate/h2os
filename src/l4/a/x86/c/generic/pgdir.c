#include <l4/generic/pgdir.h>
#include <l4/generic/allocpage.h>
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
	memset(&pgdir->pd + PdeIndex(KernVirtEnd), 0, PgdirSize - PdeIndex(KernVirtEnd) * EntrySize);
}

void pgdir_switch(struct pgdir *pgdir, struct ipc_buf *ipcbuf_ptr)
{
	pa_t ipcbuf = (pa_t)ipcbuf_ptr;
	pa_t pd = (pa_t)&pgdir->pd;
	//printk("pd=%p, ipcbuf=%p", pd, ipcbuf);

	BUG_ON(!ipcbuf || PageOffset(ipcbuf));

	kPtes[PageNum(KernIPCBuffer)] = Pte(ipcbuf, PtePerm_UserRW);

	if (mmu_getPgdirPaddr() == pd) {
		//printk("!!set ipcbuf to %d", ipcbuf);
		mmu_invalidatePage(KernIPCBuffer);

	} else {
		BUG_ON(!pd || PgdirOffset(pd));
		mmu_setPgdirPaddr(pd);
	}
}

#if 0
static pte_t *pgtab_forky(va_t va0, pte_t *pt)
{
	pte_t *new_pt = (pte_t*)alloc_page();
	memcpy(new_pt, pt, PgtabSize * EntrySize);
	pt = new_pt;
	int i;
	for (i = 0; i < PgtabSize; i++) {
		pde_t pte = pt[i];
		if (~pte & (PG_P|PG_W))
			continue;
	//printk("got pte=%p", pte);
		//void *pg = (void*)(pte & PGMASK);
		void *pg = (void*)(va0 + i * PageSize);
		//printk("pg=%p", pg);
		void *new_pg = (void*)alloc_page();
		memcpy(new_pg, pg, PageSize);
		pt[i] = (va_t)new_pg | (pte & ~PGMASK);
	}
	return pt;
}

static void pgd_forkcpy(pde_t *dst_pd, pde_t *src_pd)
{
	BUG_ON((pa_t)src_pd != mmu_getPgdirPaddr());
	memcpy(dst_pd, src_pd, PgdirSize * EntrySize);
	pde_t *pd = dst_pd;
	int i;
	for (i = PdeIndex(KernVirtEnd); i < PgdirSize; i++) {
		pde_t pde = pd[i];
		if (~pde & (PG_P|PG_W))
			continue;
		BUG_ON(pde & PG_PSE);
		pte_t *pt = (pte_t*)(pde & PGMASK);
		pt = pgtab_forky(i * PageSize * PgtabSize, pt);
		pd[i] = (va_t)pt | (pde & ~PGMASK);
	}
}

struct pgdir *pgdir_fork(struct pgdir *pgdir)
{
	struct pgdir *new_pd = (void*)alloc_page();
	pgd_forkcpy(new_pd->pd, pgdir->pd);
	return new_pd;
}
#endif
