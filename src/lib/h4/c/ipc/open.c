#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <l4/api/rtalloc.h>
#include <l4/enum/rtype.h>
#include <printk.h>

int ipc_open(key_t key, unsigned int flags)
{
	//printk("ipc_open(%d, %d)", key, flags);int r =
	return sys_rt_open(RTYPE_ENDPOINT, key, flags);
	//printk("ipc_open returns %d", r);
	//return r;
}

int ipc_close(int fd)
{
	return sys_rt_close(fd);
}
