#include <stdio.h>
#include <malloc.h>

FILE *fdopen(int fd, const char *mode)
{
	FILE *fp = malloc(sizeof(FILE));
	fp->fd = fd;
	fp->err = 0;
	fp->eof = 0;
	return fp;
}
