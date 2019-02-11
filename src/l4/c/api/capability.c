#include <l4/api/capability.h>
#include <l4/generic/capability.h>

#include <l4/enum/cap-types.h>
#include <l4/enum/cap-access.h>
#include <l4/enum/errno.h>

#include <l4/api/thread.h>
#include <l4/api/endpoint.h>
#include <l4/api/ioport.h>

#include <l4/misc/bug.h>

int cap_action(struct capability *cap, unsigned int action, unsigned long argument)
{
	if (!(cap->access & CAP_ACCESS_ACTION))
		return -ENOCAP;

	switch (cap->rtype)
	{
	case CAP_RTYPE_THREAD:
		return cap_thread_action(cap, action, argument);
	case CAP_RTYPE_ENDPOINT:
	case CAP_RTYPE_SPACE:
	case CAP_RTYPE_IOPORT:
		return -ENOSYS;
	default:
		BUG();
	}
}

int cap_send(struct capability *cap, unsigned long addr, const char *buf, size_t size)
{
	if (!(cap->access & CAP_ACCESS_WRITE))
		return -ENOCAP;
	if (cap_verify(cap, addr) < 0)
		return -ENOCAP;
	addr = cap_address(cap, addr);

	switch (cap->rtype)
	{
	case CAP_RTYPE_ENDPOINT:
		return cap_endpoint_send(cap, addr, buf, size);
	case CAP_RTYPE_THREAD:
	case CAP_RTYPE_SPACE:
		return -ENOSYS;
	case CAP_RTYPE_IOPORT:
		return cap_ioport_send(cap, addr, buf, size);
	default:
		BUG();
	}
}

int cap_recv(struct capability *cap, unsigned long addr, char *buf, size_t size)
{
	if (!(cap->access & CAP_ACCESS_READ))
		return -ENOCAP;
	if (cap_verify(cap, addr) < 0)
		return -ENOCAP;
	addr = cap_address(cap, addr);

	switch (cap->rtype)
	{
	case CAP_RTYPE_THREAD:
		return cap_endpoint_recv(cap, addr, buf, size);
	case CAP_RTYPE_SPACE:
		return -ENOSYS;
	case CAP_RTYPE_IOPORT:
		return cap_ioport_recv(cap, addr, buf, size);
	default:
		BUG();
	}
}
