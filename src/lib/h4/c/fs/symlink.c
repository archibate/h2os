#include <h4/fs.h>
#include <h4/fs/dirent.h>
#include <string.h>
#include <errno.h>

int symlink(const char *oldpath, const char *newpath)
{
	int fd = open(newpath, O_CREAT | O_EXCL | T_LNK);
	if (fd < 0)
		return fd;
	size_t len = strlen(oldpath);
	int ret = write(fd, oldpath, len);
	close(fd);
	if (ret < 0)
		return ret;
	if (ret != len)
		return -ENAMETOOLONG;
	return 0;
}
