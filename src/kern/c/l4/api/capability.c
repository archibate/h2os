#include <l4/api/capability.h>
#include <l4/api/cap-actions.h>
#include <l4/api/cap-access.h>

static int cap_thread_action(struct capability *cap, unsigned int action, unsigned long argument);

int cap_action(struct capability *cap, unsigned int action, unsigned long argument)
{
	if (!(cap->access & CAP_ACCESS_ACTION))
		return -ENOCAP;

	switch (cap->rtype)
	{
	case CAP_RTYPE_THREAD:
		return cap_thread_action(action, argument);
	case CAP_RTYPE_ENDPOINT:
	case CAP_RTYPE_SPACE:
	case CAP_RTYPE_IOPORT:
		return -ENOSYS;
	default:
		BUG();
	}
}

int cap_thread_action(struct capability *cap, unsigned int action, unsigned long argument)
{
	switch (action)
	{
	case THREAD_SUSPEND:
		return thread_suspend(cap);
	case THREAD_ACTIVE:
		return thread_active(cap);
	case THREAD_SET_PRIORITY:
		return thread_set_priority(cap, argument);
	default:
		return -ENOSYS;
	}
}


static int cap_endpoint_send(struct capability *cap, unsigned long addr, const char *buf, size_t size);
static int cap_ioport_send(struct capability *cap, unsigned long addr, const char *buf, size_t size);
static int cap_endpoint_recv(struct capability *cap, unsigned long addr, char *buf, size_t size);
static int cap_ioport_recv(struct capability *cap, unsigned long addr, char *buf, size_t size);

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
		return cap_endpoint_send(cap, action, argument);
	case CAP_RTYPE_THREAD:
	case CAP_RTYPE_SPACE:
		return -ENOSYS;
	case CAP_RTYPE_IOPORT:
		return cap_ioport_send(cap, action, argument);
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
		return cap_endpoint_recv(cap, addr, action, argument);
	case CAP_RTYPE_SPACE:
		return -ENOSYS;
	case CAP_RTYPE_IOPORT:
		return cap_ioport_recv(cap, addr, action, argument);
	default:
		BUG();
	}
}

int cap_ioport_send(struct capability *cap, unsigned long addr, const char *buf, size_t size)
{
	unsigned short port = addr;
	while (size--)
		outb(port, *buf++);
}

int cap_ioport_recv(struct capability *cap, unsigned long addr, char *buf, size_t size)
{
	unsigned short port = addr;
	while (size--)
		*buf++ = inb(port);
}

int cap_endpoint_send(struct capability *cap, unsigned long addr, const char *buf, size_t size)
{
	struct endpoint *ep;
	ep = id_get(endpoint_ids, cap->resid);
	BUG_ON(!ep);

	endpoint_call(ep, current, false, false);
}

int cap_endpoint_recv(struct capability *cap, unsigned long addr, char *buf, size_t size)
{
	struct endpoint *ep;
	ep = id_get(endpoint_ids, cap->resid);
	BUG_ON(!ep);

	endpoint_wait(ep, current);
}
