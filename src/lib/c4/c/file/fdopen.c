#include <stdio.h>
#include <malloc.h>
#include <string.h>

FILE *fdopen(int fd, const char *mode)
{
	FILE *fp = malloc(sizeof(FILE));
	fp->fd = fd;
	fp->err = 0;
	fp->eof = 0;
	fp->wr = NULL != strchr(mode, 'w');
	return fp;
}
