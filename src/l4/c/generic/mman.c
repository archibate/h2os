#include <l4/generic/mman.h>
#include <l4/generic/mregion.h>
#include <l4/generic/kcache.h>
#include <l4/generic/allocpage.h>
#include <l4/machine/mmu/page.h>
#include <l4/generic/allocpage.h>
#include <bug.h>

struct kcache mreg_kcache;

void init_mman(void)
{
	kcache_init(&mreg_kcache, sizeof(struct mregion), PageSize);
}

struct mregion *mm_new(struct mm *mm, word_t start, word_t end)
{
	struct mregion *mreg;
	hlist_for_each_entry2(mreg, &mm->mregs, hlist) {
		if (mreg_hasisect(mreg, start, end))
			return NULL;
	}

	mreg = kcache_alloc(&mreg_kcache);
	if (mreg == NULL) {
		kcache_add_page(&mreg_kcache, alloc_page());
		mreg = kcache_alloc(&mreg_kcache);
		BUG_ON(mreg == NULL);
	}

	hlist_add_head(&mreg->hlist, &mm->mregs);

	mreg->start = start;
	mreg->end = end;

	return mreg;
}

struct mregion *mm_lookup(struct mm *mm, word_t addr)
{
	struct mregion *mreg;
	hlist_for_each_entry2(mreg, &mm->mregs, hlist) {
		if (mreg_inside(mreg, addr, addr + 1))
			return mreg;
	}
	return NULL;
}
