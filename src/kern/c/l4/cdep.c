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
	cderef(cap->parent);
}

void cderef(cap_t *cap)
{
	assert(cap->refcnt);
	if (--cap->refcnt <= 0)
		cdelete(cap);
}
