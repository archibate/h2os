#include <l4/api/endpoint.h>
#include <l4/generic/endpoint.h>
#include <l4/generic/cap-convert.h>
#include <l4/generic/thread.h>

int cap_endpoint_send(struct capability *cap, unsigned long addr, const char *buf, size_t size)
{
	struct endpoint *ep = cap_get_endpoint(cap);
	endpoint_call(ep, current, false, false);
	return 0;
}

int cap_endpoint_recv(struct capability *cap, unsigned long addr, char *buf, size_t size)
{
	struct endpoint *ep = cap_get_endpoint(cap);
	endpoint_wait(ep, current);
	return 0;
}
