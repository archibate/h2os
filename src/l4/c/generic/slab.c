#include <l4/generic/slab.h>
#include <l4/generic/mem.h>
#include <l4/generic/rtmethod.h>
#include <l4/enum/rtype.h>
#include <l4/misc/bug.h>

#define SLAB_BUG_VALIDICY(slab) \
	BUG_ON((slab)->rsize == 0); \
	BUG_ON((slab)->page == NULL); \
	BUG_ON((slab)->rsize != rtype_get_sizeof((slab)->rtype));

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

	pa_t pa = mem_alloc_frame(mem);
	void *page = map_virt_page(pa);
	BUG_ON(!page);

	slab->rtype = rtype;
	slab->rsize = rsize;
	slab->page = page;
	slab->used = 0;
}

void slab_delete(struct slab *slab)
{
	if (slab->page == NULL)
		return;

	slab_revoke(slab);
	unmap_virt_page(slab->page);
	slab->page = 0;
}

void *slab_alloc(struct slab *slab)
{
	SLAB_BUG_VALIDICY(slab);

	size_t offset = slab->used * slab->rsize;
	if (offset > PageSize - slab->rsize)
		return NULL;
	slab->used++;

	void *p = slab->page + offset;
	rtype_init(p, slab->rtype);
	return p;
}

void slab_revoke(struct slab *slab)
{
	SLAB_BUG_VALIDICY(slab);

	unsigned int i;
	for (i = 0; i < slab->used; i++) {
		void *p = slab->page + i * slab->rsize;
		rtype_delete(p, rtype);
	}
}
