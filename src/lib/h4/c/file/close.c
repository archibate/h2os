#include <h4/file/api.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

int close(int fd)
{
	ipc_rewindw(_FILE_close);
	ipc_call(fd);
	return ipc_getw();
}
