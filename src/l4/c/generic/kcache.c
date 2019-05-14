#include <l4/generic/kcache.h>
#include <l4/generic/allocpage.h>
#include <l4/lib/bmptools.h>
#include <l4/misc/bug.h>
#include <l4/machine/mmu/page.h>

#define PGSIZE              PageSize
#define NODE_SIZE           sizeof(struct slab_node) // 4*4 = 16-bytes
#define kc_node_size(kc)    (NODE_SIZE + ((kc)->rcount + 7) / 8)

void kcache_init(struct kcache *kc, size_t rsize)
{
	size_t rcount = PGSIZE - NODE_SIZE;//32640/319
	rcount = (rcount * 8) / (8 * rsize + 1);
	assert(rcount >= 2);
/* WHY? {{{
rcount = (PGSIZE - NODE_SIZE - rcount / 8) / rsize
rcount * rsize = PGSIZE - NODE_SIZE - 1/8 * rcount
(rsize + 1/8) * rcount = PGSIZE - NODE_SIZE
rcount = (PGSIZE - NODE_SIZE) / (rsize + 1/8)
rcount = (PGSIZE - NODE_SIZE)*8 / (8*rsize + 1)
}}}*/

	kc->rsize = rsize;
	kc->rcount = rcount;
	assert(kc->rsize * kc->rcount + kc_node_size(kc) <= PGSIZE);

	hlist_head_init(&kc->slab_full);
	hlist_head_init(&kc->slab_partial);
	hlist_head_init(&kc->slab_empty);
}

static void kcache_add_page(struct kcache *kc, void *page);
static void *__kcache_alloc(struct kcache *kc);

void *kcache_alloc(struct kcache *kc)
{
	void *p = __kcache_alloc(kc);
	if (p == NULL) {
		kcache_add_page(kc, alloc_page());
		p = __kcache_alloc(kc);
		BUG_ON(p == NULL);
	}
	return p;
}

#define kc_slab_page(kc, slab) ((void*)(slab) + kc_node_size(kc))

void kcache_add_page(struct kcache *kc, void *page)
{
	struct slab_node *slab = page;
	slab->usage = 0;
	hlist_add_head(&slab->hlist, &kc->slab_empty);
}

static int bmp_first_zero_set(char *bmp, size_t max);

void *__kcache_alloc(struct kcache *kc)
{
	struct slab_node *slab;

	if (!hlist_empty(&kc->slab_partial)) {
		struct hlist_node *node = kc->slab_partial.first;
		slab = hlist_entry(node, struct slab_node, hlist);

		if (slab->usage + 1 >= kc->rcount) {
			hlist_move(node, &kc->slab_full);
		}

	} else if likely(!hlist_empty(&kc->slab_empty)) {
		struct hlist_node *node = kc->slab_empty.first;
		slab = hlist_entry(node, struct slab_node, hlist);

		hlist_move(node, &kc->slab_partial);

	} else {
		//BUG();
		return NULL;
	}

	slab->usage++;

	void *page = kc_slab_page(kc, slab);
	int n = bmp_first_zero_set(slab->bitmap, kc->rcount);
	void *p = page + n * kc->rsize;

	BUG_ON(p == NULL);
	return p;
}

int bmp_first_zero_set(char *bmp, size_t max)
{
	int i;
	for (i = 0; i < max; i++) {
		if (!bmp_getbit(bmp, i)) {
			bmp_setbit(bmp, i);
			return i;
		}
	}
	BUG();
}

void kcache_free(struct kcache *kc, void *p)
{
	unsigned long addr, off;
	struct slab_node *slab;
	int n;

	addr = (unsigned long)p;
	off = addr % PGSIZE;
	slab = (void*)(addr - off);
	n = off / kc->rsize;

	BUG_ON(!bmp_getbit(slab->bitmap, n));
	bmp_clrbit(slab->bitmap, n);

	if (slab->usage <= 1) {
		hlist_move(&slab->hlist, &kc->slab_empty);

	} else if (slab->usage == kc->rcount) {
		hlist_move(&slab->hlist, &kc->slab_partial);
	}

	slab->usage--;
}
