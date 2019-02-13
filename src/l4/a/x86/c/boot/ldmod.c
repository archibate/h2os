#include <l4/boot/ldmod.h>
#include <l4/boot/ldelf.h>
#include <l4/boot/alloc.h>
#include <l4/generic/utcb.h>
#include <l4/generic/pgdir.h>
#include <l4/machine/mmu/page.h>
#include <l4/system/kstack.h>
#include <l4/misc/panic.h>
#include <l4/generic/thread.h>
#include <l4/enum/thread-states.h>

void make_simple_thread(struct ktcb *tcb,
		struct utcb *utcb,
		struct pgdir *pgdir)
{
	tcb->utcb = utcb;
	tcb->pgdir = pgdir;
	tcb->state = THREAD_RUNNING;
	thread_active(tcb);
}

void load_module(const void *begin, const void *end)
{
	struct utcb *utcb = calloc(1, PageSize);
	struct pgdir *pgdir = calloc(1, PageSize);
	struct ktcb *tcb = calloc(1, sizeof(struct ktcb));
	make_simple_thread(tcb, utcb, pgdir);

	pgdir_init(pgdir);
	utcb_init(utcb);
	pgdir_switch(pgdir, utcb);

	void *pc;
	if (!(pc = loadelf(begin, end)))
		panic("bad module ELF format");

	utcb->iframe.pc = (word_t)pc;
}
