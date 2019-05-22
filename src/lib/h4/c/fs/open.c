#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/fs/api.h>
#include <h4/fs/sysnr.h>
#include <h4/filedes.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <printk.h>//

int open(const char *path, int flags)
{
	int fd = ipc_dup(SVFD_FS);
	//printk("ipc_dup: fd=%d", fd);
	if (fd < 0)
		return fd;
	ipc_rewindw(_FS_open);
	ipc_putw(flags);
	ipc_write(path, strlen(path) + 1);
	ipc_call(fd);
	int ret = ipc_getw();
	//printk("ipc_call: ret=%d", ret);
	if (ret == 0)
		return fd;
	ipc_close(fd);
	if (ret < 0)
		return ret;
	//printk("ipc_open(%d)", ret);
	return ipc_open(ret);
}

#if 0//{{{
int openat(int dirfd, const char *path, unsigned int flags)
{
	ipc_rewindw(_FS_open);
	ipc_putw(flags);
	ipc_write(path, strlen(path) + 1);
	int ret = ipc_dup2(SVFD_FS, dirfd);
	if (ret < 0)
		return ret;
	ipc_call(dirfd);
	ret = ipc_getw();
	if (ret <= 0)
		return ret;
	ipc_close(dirfd);
	int fd = ipc_open(ret);
	if (fd < 0)
		return fd;
	return ipc_dup2(fd, dirfd);
}
#endif//}}}
