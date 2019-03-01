#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <l4/enum/rtype.h>

int ipc_open(key_t key, unsigned int flags)
{
	return sys_rt_open(RTYPE_ENDPOINT, key, flags);
}

int ipc_close(int epid)
{
	//return sys_rt_close(epid);
}