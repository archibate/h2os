#include <stdio.h>
#include <h4/file.h>

int fclose(FILE *fp)
{
	int fd = fp->fd;
	fdclose(fp);
	return close(fd);
}
