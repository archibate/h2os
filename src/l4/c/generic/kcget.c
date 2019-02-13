#include <l4/generic/kcget.h>
#include <l4/generic/kcache.h>
#include <l4/generic/rtmethod.h>
#include <l4/machine/mmu/page.h>
#include <l4/enum/rtype.h>
#include <l4/boot/alloc.h>
#include <l4/misc/printk.h>
#include <l4/misc/bug.h>

#define ONCE_FEED_PAGES 2

struct kcache kcaches[RTYPE_MAX];

void init_kcg(void)
{
	unsigned int i;
	for (i = 0; i < RTYPE_MAX; i++) {
		size_t rsize = rtype_get_sizeof(i);
		kcache_init(&kcaches[i], rsize, PageSize);
	}
}

void *kcg_new(unsigned int rtype)
{
	BUG_ON(rtype >= RTYPE_MAX);
	struct kcache *kc = &kcaches[rtype];

	void *p = kcache_alloc(kc);
	if (p == NULL) {
		unsigned int i;
		for (i = 0; i < ONCE_FEED_PAGES; i++) {
			void *page = calloc(1, PageSize);
			kcache_add_page(kc, page);
		}

		p = kcache_alloc(kc);
		BUG_ON(p == NULL);
	}

	rtype_init(p, rtype);
	return p;
}

void kcg_delete(unsigned int rtype, void *p)
{
	BUG_ON(rtype >= RTYPE_MAX);

	rtype_delete(p, rtype);
	kcache_free(&kcaches[rtype], p);
}
