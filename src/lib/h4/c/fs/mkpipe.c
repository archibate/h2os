#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/fs/api.h>
#include <h4/fs/sysnr.h>
#include <h4/filedes.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <printk.h>//

int mkpipe(void)
{
	int fd = ipc_dup(SVFD_FS);
	if (fd < 0)
		return fd;
	ipc_rewindw(_FS_pipe);
	ipc_call(fd);
	int ret = ipc_getw();
	if (ret >= 0)
		return fd;
	ipc_close(fd);
	return ret;
}
