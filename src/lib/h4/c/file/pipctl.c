#include <h4/file.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

int pipctl(int fd, int iswr)
{
	ipc_rewindw(_FILE_pipctl);
	ipc_putw(iswr);
	ipc_call(fd);
	return ipc_getw();
}
