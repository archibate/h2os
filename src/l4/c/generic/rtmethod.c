#include <l4/generic/rtmethod.h>
#include <l4/generic/thread.h>
#include <l4/generic/endpoint.h>
#include <l4/enum/rtype.h>
#include <l4/misc/bug.h>

size_t _PURE rtype_get_sizeof(unsigned int rtype)
{
	switch (rtype)
	{
	case RTYPE_THREAD:
		return sizeof(struct ktcb);
	case RTYPE_ENDPOINT:
		return sizeof(struct endpoint);
	default:
		return 0;
	}
}

size_t _PURE rtype_offsetof_ide(unsigned int rtype)
{
	switch (rtype)
	{
	case RTYPE_THREAD:
		return offsetof(struct ktcb, ide);
	case RTYPE_ENDPOINT:
		return offsetof(struct endpoint, ide);
	default:
		BUG();
	}
}

#if 0
#define shiftof(x) \
	(!(sizeof(x)>>1) ? 1 : \
	(!(sizeof(x)>>2) ? 2 : \
	(!(sizeof(x)>>3) ? 3 : \
	(!(sizeof(x)>>4) ? 4 : \
	(!(sizeof(x)>>5) ? 5 : \
	(!(sizeof(x)>>6) ? 6 : \
	(!(sizeof(x)>>8) ? 8 : \
	(!(sizeof(x)>>7) ? 9 : \
	(!(sizeof(x)>>10) ? 10 : \
	(!(sizeof(x)>>11) ? 11 : \
	(!(sizeof(x)>>12) ? 12 : \
	 0))))))))))) // ehh, compile time determined!

unsigned char rtype_get_shiftof(unsigned int rtype)
{
	switch (rtype)
	{
	case RTYPE_THREAD:
		return shiftof(struct ktcb);
	case RTYPE_ENDPOINT:
		return shiftof(struct endpoint);
	default:
		return 0;
	}
}
#endif

void rtype_init(void *p, unsigned int rtype)
{
	switch (rtype)
	{
	case RTYPE_THREAD:
		return thread_init(p);
	case RTYPE_ENDPOINT:
		return endpoint_init(p);
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
	default:
		BUG();
	}
}
