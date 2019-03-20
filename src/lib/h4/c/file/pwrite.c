#include <h4/file/api.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

ssize_t pwrite(int fd, const void *buf, size_t len, off_t off)
{
	ipc_rewindw(_FILE_pwrite);
	ipc_putw(len);
	ipc_putw(off);
	ipc_write(buf, len);
	ipc_call(fd);
	return ipc_getw();
}
