#include <l4/api/asyncep.h>
#include <l4/generic/sched.h>
#include <l4/generic/asyncep.h>
#include <l4/generic/idspace.h>
#include <l4/enum/errno.h>

IDSPACE(async_ep);

int sys_async_pulse(l4id_t id)
{
	struct async_ep *aep = ID(async_ep, id);
	if (!aep)
		return -ESRCH;

	async_pulse(aep);
	return 0;
}

int sys_async_poll(l4id_t id)
{
	struct async_ep *aep = ID(async_ep, id);
	if (!aep)
		return -ESRCH;

	bool r = async_poll(aep);
	return r ? 1 : 0;
}

int sys_async_listen(l4id_t id)
{
	struct async_ep *aep = ID(async_ep, id);
	if (!aep)
		return -ESRCH;

	async_listen(aep, current);
	return 0;
}
