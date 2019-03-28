#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/fs/api.h>
#include <h4/fs/sysnr.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <printk.h>

int fs_open(int fs, const char *path, unsigned int flags)
{
	ipc_rewindw(_FS_open);
	ipc_putw(flags);
	ipc_write(path, strlen(path) + 1);
	int fd = ipc_dup(fs);
	if (fd < 0)
		return fd;
	ipc_call(fd);
	int ret = ipc_getw();
	if (ret < 0)
		return ret;
	else if (ret == 0)
		return fd;
	ipc_close(fd);
	return ipc_open(ret);
}

#if 0//{{{
int fs_openat(int fs, int dirfd, const char *path, unsigned int flags)
{
	ipc_rewindw(_FS_open);
	ipc_putw(flags);
	ipc_write(path, strlen(path) + 1);
	int fd = ipc_callfdat(fs, dirfd);
	int ret = ipc_getw();
	return ret < 0 ? ret : fd;
}
#endif//}}}
