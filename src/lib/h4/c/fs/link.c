#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/fs/api.h>
#include <h4/fs/sysnr.h>
#include <h4/filedes.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <printk.h>//

int link(const char *oldpath, const char *newpath)
{
	ipc_rewindw(_FS_link);
	ipc_write(oldpath, strlen(oldpath) + 1);
	ipc_write(newpath, strlen(newpath) + 1);
	ipc_call(SVFD_FS);
	return ipc_getw();
}
