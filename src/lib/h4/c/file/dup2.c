#include <h4/file/api.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

int dup2(int fd, int dirfd)
{
	ipc_dup2(fd, dirfd);
}

