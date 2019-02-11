#include <l4/generic/capability.h>
#include <l4/misc/bug.h>
#include <errno.h>
#include <memory.h>

void cap_init(struct capability *cap)
{
	memset(cap, 0, sizeof(*cap));
	hlist_node_init(&cap->hlist);
}

int cap_consume(struct capability *cap, unsigned long quantity)
{
	if (cap->size < cap->used + quantity)
		return -ENOCAP;

	cap->used += quantity;
	return 0;
}

int cap_free(struct capability *cap, unsigned long quantity)
{
	BUG_ON(cap->used < quantity);
	cap->used -= quantity;
	return 0;
}

int cap_limit(struct capability *cap, unsigned long base, unsigned long size)
{
	cap->base += base;
	if (size < cap->size)
		cap->size = size;
	return 0;
}

int cap_verify(struct capability const *cap, unsigned long addr)
{
	if (addr >= cap->size)
		return -ENOCAP;
	return 0;
}

unsigned long cap_address(struct capability const *cap, unsigned long addr)
{
	return cap->base + addr;
}
