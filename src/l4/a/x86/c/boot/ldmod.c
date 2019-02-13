#include <l4/boot/ldmod.h>
#include <l4/boot/ldelf.h>
#include <l4/boot/alloc.h>
#include <l4/generic/utcb.h>
#include <l4/generic/pgdir.h>
#include <l4/machine/mmu/page.h>
#include <l4/system/kstack.h>
#include <l4/misc/panic.h>
#include <l4/generic/thread.h>

#include <l4/generic/slab.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/idspace.h>
void init_ldmod(void)
{
	unsigned int i;
	for (i = 0; i < RTYPE_MAX; i++)
		ids_init(&idspaces0[i]);
	slab_init(rtype_sizeof(r
}

void make_simple_thread(struct ktcb *tcb,
		struct utcb *utcb,
		struct pgdir *pgdir,
		struct slab *slab)
{
	static struct id_space idspaces0[RTYPE_MAX];
	tcb->utcb = utcb;
	tcb->pgdir = pgdir;
	unsigned int i;
	for (i = 0; i < RTYPE_MAX; i++)
		tcb->slabs[i] = slab;
	for (i = 0; i < RTYPE_MAX; i++)
		tcb->idspaces[i] = &idspaces0[i];
	tcb->state = THREAD_RUNNING;
	thread_active(tcb);
}

void load_module(const void *begin, const void *end)
{
	struct utcb *utcb = calloc(1, PageSize);
	struct pgdir *pgdir = calloc(1, PageSize);
	struct ktcb *tcb = calloc(1, sizeof(struct ktcb));
	make_simple_thread(tcb, utcb, pgdir, slab);

	pgdir_init(pgdir);
	utcb_init(utcb);
	pgdir_switch(pgdir, utcb);

	void *pc;
	if (!(pc = loadelf(begin, end)))
		panic("bad module ELF format");

	utcb->iframe.pc = (word_t)pc;
}
