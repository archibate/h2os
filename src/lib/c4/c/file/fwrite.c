#include <stdio.h>
#include <unistd.h>
#include <errno.h>

ssize_t fwrite(const void *buf, size_t size, size_t nmemb, FILE *fp)
{
	if (!fp->wr)
		return -EPERM;

	size_t len = size * nmemb;
	if (fp->n + len < BUFSIZ) {
		memcpy(fp->b + fp->n, buf, len);
		fp->n += len;
		return nmemb;
	}
	int ret = fflush(fp);
	if (ret < 0)
		return ret;
	ssize_t r = write(fp->fd, buf, len);
	if (r < 0) {
		errno = fp->err = -r;
		return r;
	}
	return r;
}
