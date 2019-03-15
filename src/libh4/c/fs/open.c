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
	//printk("path=%s", path);
	size_t len = strlen(path) + 1;
	//printk("len=%d", len);
	ipc_write(path, len);
	ipc_call(fs);
	int id = ipc_getw();
	//printk("id=%d", id);
	if (id < 0)
		return id;
	int ret = ipc_connect(id);
	//printk("ret=%d", ret);
	if (ret < 0)
		return ret;
	return ret;
}

#if 0 // {{{
int fs_openat(int fs, int fd, const char *path, unsigned int flags)
{
	ipc_rewindw(_FS_open);
	ipc_putw(flags);
	size_t len = strlen(path) + 1;
	char *buf = ipc_getbuf(&len);
	memcpy(buf, path, len);
	ipc_call(fs);
	int id = ipc_getw();
	if (id < 0)
		return id;
	return ipc_connectat(id, fd);
}
#endif // }}}
