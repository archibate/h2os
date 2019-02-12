#include <l4/generic/rtmethod.h>
#include <l4/generic/thread.h>
#include <l4/generic/endpoint.h>
#include <l4/generic/slab.h>
#include <l4/enum/rtype.h>
#include <l4/misc/bug.h>

size_t rtype_get_sizeof(unsigned int rtype)
{
	switch (rtype)
	{
	case RTYPE_THREAD:
		return sizeof(struct ktcb);
	case RTYPE_ENDPOINT:
		return sizeof(struct endpoint);
	case RTYPE_SLAB:
		return sizeof(struct slab);
	default:
		return 0;
	}
}

void rtype_init(void *p, unsigned int rtype)
{
	switch (rtype)
	{
	case RTYPE_THREAD:
		return thread_init(p);
	case RTYPE_ENDPOINT:
		return endpoint_init(p);
	case RTYPE_SLAB:
		return slab_init(p);
	default:
		BUG();
	}
}

void rtype_revoke(void *p, unsigned int rtype)
{
	switch (rtype)
	{
	case RTYPE_THREAD:
		return thread_revoke(p);
	case RTYPE_ENDPOINT:
		return endpoint_revoke(p);
	case RTYPE_SLAB:
		return slab_revoke(p);
	default:
		BUG();
	}
}

void rtype_delete(void *p, unsigned int rtype)
{
	switch (rtype)
	{
	case RTYPE_THREAD:
		return thread_delete(p);
	case RTYPE_ENDPOINT:
		return endpoint_delete(p);
	case RTYPE_SLAB:
		return slab_delete(p);
	default:
		BUG();
	}
}
