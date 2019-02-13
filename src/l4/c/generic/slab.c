#include <l4/generic/slab.h>
#include <l4/generic/rtmethod.h>
#include <l4/misc/bug.h>

#define NODE_SIZE sizeof(struct hlist_node)

void *slab_new(struct slab *slab)
{
	void *p = slab->curr;
	BUG_ON((word_t)p % slab->rsize != 0);
	if (p + slab->rsize >= slab->end)
		return NULL;
	slab->curr += slab->rsize;
	return p;
	/*if (hlist_empty(&slab->f))
		return NULL;
	void *p = __hlist_pop(&slab->f);
	return p;*/
}

void slab_free(struct slab *slab, void *p)
{
	BUG_ON((word_t)p % slab->rsize != 0);
	/*struct hlist_node *curr;
	for (curr = slab->f.first; curr; curr = curr->next)
		if (p > (void*)curr)
			break;
	hlist_add_after(curr, p);*/
}

#if 0
void __slab_rt_revoke(struct slab *slab, unsigned int rtype)
{
	BUG_ON((word_t)p % NODE_SIZE != 0);
	struct hlist_node *curr;
	for (curr = slab->f.first; curr->next; curr = curr->next) {
		curr->next
	}
}
#endif

void slab_init(struct slab *slab, void *begin, void *end, size_t rsize)
{
	if (rsize < NODE_SIZE)
		rsize = NODE_SIZE;

	BUG_ON((word_t)begin % rsize != 0);
	BUG_ON((word_t)end % rsize != 0);
	slab->begin = begin;
	slab->curr = begin;
	slab->end = end;
	slab->rsize = rsize;

#if 0
	void *p;
	struct hlist_node *curr, **pprev = &slab->f.first;
	for (p = begin; p < end; p += rsize) {
		curr = p;
		curr->pprev = pprev;
		*pprev = curr;
		pprev = &curr->next;
	}
	*pprev = 0;
#endif
}
