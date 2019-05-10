#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

FILE *fopen(const char *name, const char *mode)
{
	int m = 0;
	if (strchr(mode, 'r')) m |= O_RDONLY;
	if (strchr(mode, 'w')) m |= O_WRONLY;
	if (strchr(mode, 'a')) m |= O_WRONLY | O_APPEND;
	if (strchr(mode, '+')) m |= O_CREAT;
	int fd = open(name, m);
	if (fd < 0) {
		errno = -fd;
		return NULL;
	}
	return fdopen(fd, mode);
}
