#include <l4/generic/endpoint.h>
#include <memory.h>

void endpoint_init(struct endpoint *ep)
{
	memset(ep, 0, sizeof(*ep));
}

void endpoint_revoke(struct endpoint *ep)
{
}

void endpoint_delete(struct endpoint *ep)
{
	endpoint_revoke(ep);
	hlist_del(&ep->ide.hlist);
}
