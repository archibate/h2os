#include <stdio.h>
#include <h4/file/api.h>
#include <errno.h>

ssize_t fwrite(const void *buf, size_t size, size_t nmemb, FILE *fp)
{
	ssize_t ret = write(fp->fd, buf, size * nmemb) / size;
	if (ret == 0) {
		fp->eof = 1;
	} else if (ret < 0) {
		errno = fp->err = -ret;
		ret = 0;
	}
	return ret;
}
