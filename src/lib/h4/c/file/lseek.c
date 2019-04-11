#include <h4/file.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

off_t lseek(int fd, off_t off, int whence)
{
	ipc_rewindw(_FILE_lseek);
	ipc_putw(off);
	ipc_putw(whence);
	ipc_call(fd);
	return ipc_getw();
}
