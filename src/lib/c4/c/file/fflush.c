#include <stdio.h>
#include <errno.h>
#include <bug.h>

int fflush(FILE *fp)
{
	if (!fp->wr || fp->n == 0)
		return 0;
	ssize_t r = write(fp->fd, fp->b, fp->n);
	if (r < 0) {
		errno = fp->err = -r;
		return r;
	}
	BUG_ON(r != fp->n);
	fp->p = fp->b;
	fp->n = 0;
	return r;
}
