#include <h4/fs/pipe.h>
#include <h4/file.h>

int pipe(int fd[2])
{
	fd[0] = mkpipe();
	if (fd[0] < 0)
		return fd[0];
	fd[1] = dup(fd[0]);
	if (fd[1] < 0) {
		close(fd[0]);
		return fd[1];
	}
	int ret = pipctl(fd[0], 0);
	if (ret >= 0)
		ret = pipctl(fd[1], 1);
	return ret;
}

