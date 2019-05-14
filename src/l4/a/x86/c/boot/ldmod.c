#include <l4/boot/ldmod.h>
#include <l4/boot/ldelf.h>
#include <l4/generic/allocpage.h>
#include <l4/generic/kcache.h>
#include <l4/generic/mman.h>
#include <l4/generic/context.h>
#include <l4/generic/pgdir.h>
#include <l4/generic/idspace.h>
#include <l4/machine/mmu/page.h>
#include <l4/system/kstack.h>
#include <l4/misc/panic.h>
#include <l4/generic/thread.h>
#include <l4/enum/thread-states.h>
#include <l4/generic/idspace.h>
#include <memory.h>

IDSPACE(mm);

l4id_t load_module(const void *begin, const void *end)
{
	struct ktcb *tcb = new_thread();
	struct mm *mm = KCNEW(mm);
	mm_init(mm);
	l4id_t id = IDNEW(mm, mm);

	pgdir_switch(mm->pgdir, tcb->ipcbuf);
	if (!(tcb->context.pc = (word_t)loadelf(begin, end)))
		panic("bad module ELF format");

	tcb->mm = mm;
	LIDNEW(mm, ktcb, tcb);

	tcb->state = THREAD_RUNNING;
	thread_active(tcb);

	return id;
}
