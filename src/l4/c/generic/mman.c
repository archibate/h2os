#include <l4/generic/mman.h>
#include <l4/generic/mregion.h>
#include <l4/generic/pgdir.h>
#include <l4/generic/kcache.h>
#include <l4/generic/idspace.h>
#include <l4/generic/allocpage.h>
#include <l4/machine/mmu/page.h>
#include <l4/misc/bug.h>

KCACHE(mregion);

void mm_init(struct mm *mm)
{
	ids_init(&mm->lids_mm);
	ids_init(&mm->lids_ktcb);
	mm->pgdir = (struct pgdir *)alloc_page();
	pgdir_init(mm->pgdir);
}

struct mregion *mm_new_mreg(struct mm *mm, word_t start, word_t end, unsigned int prot)
{
	struct mregion *mreg;
	hlist_for_each_entry2(mreg, &mm->mregs, hlist) {
		if (mreg_hasisect(mreg, start, end))
			return NULL;
	}

	mreg = KCNEW(mregion);

	hlist_add_head(&mreg->hlist, &mm->mregs);

	mreg->start = start;
	mreg->end = end;
	mreg->prot = prot;

	return mreg;
}

struct mregion *mm_lookup_mreg(struct mm *mm, word_t addr)
{
	struct mregion *mreg;
	hlist_for_each_entry2(mreg, &mm->mregs, hlist) {
		//printk("%p->%p: %p", mreg->start, mreg->end, addr);
		if (mreg_inside(mreg, addr, addr + 0))
			return mreg;
	}
	return NULL;
}

void mm_mreg_del(struct mregion *mreg)
{
	hlist_del(&mreg->hlist);
	KCDEL(mregion, mreg);
}

void mm_destroy(struct mm *mm)
{
	pgdir_init(mm->pgdir);
	//struct ktcb *tcb; LID_FOREACH(tcb, mm, ktcb) { thread_delete(tcb); }
}
