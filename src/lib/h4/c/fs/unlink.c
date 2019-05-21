#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/fs/api.h>
#include <h4/fs/sysnr.h>
#include <h4/filedes.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <printk.h>//

int unlink(const char *path)
{
	ipc_rewindw(_FS_unlink);
	ipc_write(path, strlen(path) + 1);
	ipc_call(SVFD_FS);
	return ipc_getw();
}
