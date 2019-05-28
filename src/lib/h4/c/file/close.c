#include <h4/file.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

int close(int fd)
{
	if (fd == 1) pipctl(fd, -1);//TODO
	ipc_close(fd);
}
