#include <l4/cdep.h>
#include <assert.h>

void cdepend(cap_t *child, cap_t *parent)
{
	child->parent = parent;
	parent->refcnt++;
}

static void cderef(cap_t *cap);

void cdelete(cap_t *cap)
{
	if (cap->parent)
		cderef(cap->parent);
	cap->ctype = 0;
}

void cderef(cap_t *cap)
{
	assert(cap->refcnt);
	if (--cap->refcnt <= 0)
		cdelete(cap);
}
