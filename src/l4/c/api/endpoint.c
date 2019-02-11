#include <l4/api/endpoint.h>
#include <l4/generic/endpoint.h>
#include <l4/generic/idget.h>
#include <l4/generic/thread.h>

int sys_nbsend(l4id_t epid)
{
	struct endpoint *ep = id_get_endpoint(epid);
	endpoint_call(ep, current, false, false);
	return 0;
}

int sys_send(l4id_t epid)
{
	struct endpoint *ep = id_get_endpoint(epid);
	endpoint_call(ep, current, true, false);
	return 0;
}

int sys_call(l4id_t epid)
{
	struct endpoint *ep = id_get_endpoint(epid);
	endpoint_call(ep, current, true, true);
	return 0;
}

int sys_recv(l4id_t epid)
{
	struct endpoint *ep = id_get_endpoint(epid);
	endpoint_wait(ep, current);
	return 0;
}
