#include <l4/generic/slab.h>
#include <l4/generic/mem.h>
#include <l4/generic/rtmethod.h>
#include <l4/misc/bug.h>

void *slab_alloc(struct slab *slab)
{
	void *p;
	BUG_ON(slab->rsize == 0);
	BUG_ON(slab->page == NULL);
	BUG_ON(slab->rsize != rtype_get_sizeof(slab->rtype));

	if (slab->used + slab->rsize > PageSize)
		return NULL;
	p = page + slab->used;
	slab->used += slab->rsize;

	rtype_init(p, slab->rtype);
	return p;
}

void slab_init(struct slab *slab)
{
	slab->rtype = RTYPE_MAX;
	slab->rsize = 0;
	slab->page = 0;
	slab->used = 0;
}

int slab_retype(struct slab *slab, struct mem *mem, unsigned int rtype)
{
	size_t rsize = rtype_get_sizeof(rsize);
	if (!rtype)
		return -EINVAL;

	pa_t pa = mem_alloc_ppage(mem);
	void *page = map_new_virt_page(pa);
	BUG_ON(!page);

	slab->rtype = rtype;
	slab->rsize = rsize;
	slab->page = page;
	slab->used = 0;
}
