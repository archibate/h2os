#include <l4/generic/mman.h>
#include <l4/generic/mregion.h>
#include <l4/generic/pgdir.h>
#include <l4/generic/kcget.h>
#include <l4/enum/rtype.h>
//#include <l4/machine/mmu/page.h>
#include <bug.h>

struct mregion *mm_new(struct mm *mm, word_t start, word_t end, unsigned int prot)
{
	struct mregion *mreg;
	hlist_for_each_entry2(mreg, &mm->mregs, hlist) {
		if (mreg_hasisect(mreg, start, end))
			return NULL;
	}

	mreg = kcg_new(RTYPE_MREGION);

	hlist_add_head(&mreg->hlist, &mm->mregs);

	mreg->start = start;
	mreg->end = end;
	mreg->prot = prot;

	return mreg;
}

struct mregion *mm_lookup(struct mm *mm, word_t addr)
{
	struct mregion *mreg;
	hlist_for_each_entry2(mreg, &mm->mregs, hlist) {
		//printk("%p->%p: %p", mreg->start, mreg->end, addr);
		if (mreg_inside(mreg, addr, addr + 0))
			return mreg;
	}
	return NULL;
}

void mm_del(struct mregion *mreg)
{
	hlist_del(&mreg->hlist);
	kcg_delete(RTYPE_MREGION, mreg);
}

void mm_destroy(struct mm *mm)
{
	pgdir_init(mm->pgdir);
}

#if 0
struct mm *mm_fork(struct mm *mm)
{
	struct mm *new_mm = kcg_new(RTYPE_MMAN);
	new_mm->pgdir = pgdir_fork(mm->pgdir);
	return new_mm;
}
#endif
