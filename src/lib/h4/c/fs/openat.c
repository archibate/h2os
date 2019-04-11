#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/fs/api.h>

int openat(int dirfd, const char *path, unsigned int flags)
{
	int fd = open(path, flags);
	if (fd != dirfd) {
		int ret = dup2(fd, dirfd);
		close(fd);
		if (ret < 0)
			return ret;
	}
	return 0;
}
