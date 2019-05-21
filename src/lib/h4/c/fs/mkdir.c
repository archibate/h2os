#include <h4/fs.h>
#include <h4/fs/dirent.h>

int mkdir(const char *path, int mode)
{
	int fd = open(path, O_DIR | O_CREAT | O_EXCL | T_DIR);
	if (fd < 0)
		return fd;
	close(fd);
	return 0;
}
