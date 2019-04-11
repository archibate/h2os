#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <l4/api/keier.h>
#include <l4/api/open.h>

int ipc_openat(key_t key, int dirfd)
{
	//printk("key=%d", key);
	int fd = ipc_open(key);
	//printk("!!fd=%d", fd);
	if (fd < 0)
		return fd;
	if (fd != dirfd) {
		//printk("!!DUP");
		int ret = ipc_dup2(fd, dirfd);
		ipc_close(fd);
		//printk("!!ret=%d", ret);
		if (ret < 0)
			return ret;
	}
	//printk("!!SUCC");
	return 0;
}
