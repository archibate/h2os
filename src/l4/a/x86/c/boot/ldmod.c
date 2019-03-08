#include <l4/boot/ldmod.h>
#include <l4/boot/ldelf.h>
#include <l4/boot/calloc.h>
#include <l4/generic/allocpage.h>
#include <l4/generic/utcb.h>
#include <l4/generic/pgdir.h>
#include <l4/machine/mmu/page.h>
#include <l4/system/kstack.h>
#include <l4/misc/panic.h>
#include <l4/generic/thread.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/idget.h>
#include <memory.h>

l4id_t load_module(const void *begin, const void *end)
{
	struct pgdir *pgdir = alloc_page();
	struct utcb *utcb = alloc_page();
	struct ipc_buf *ipcbuf = alloc_page();

	utcb_init(utcb);
	memset(ipcbuf, 0, sizeof(*ipcbuf));

	pgdir_init(pgdir);
	pgdir_switch(pgdir, utcb, ipcbuf);

	void *pc;
	if (!(pc = loadelf(begin, end)))
		panic("bad module ELF format");
	utcb->iframe.pc = (word_t)pc;

	struct ktcb *tcb = calloc(1, sizeof(struct ktcb));
	__thread_init(tcb);
	tcb->utcb = utcb;
	tcb->pgdir = pgdir;
	tcb->ipcbuf = ipcbuf;

	l4id_t id = idg_new_entry(&tcb->ide, RTYPE_THREAD);
	tcb->state = THREAD_RUNNING;
	thread_active(tcb);
	return id;
}
