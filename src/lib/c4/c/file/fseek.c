#include <stdio.h>
#include <h4/file/api.h>
#include <errno.h>

off_t fseek(FILE *fp, off_t off, int whence)
{ // arguments on wheather to satat POSIX ret value!
	fflush(fp);
	off_t ret = lseek(fp->fd, off, whence);
	if (ret < 0) {
		errno = fp->err = -ret;
		for (;;);
	} else {
		fp->eof = 0;
	}
	return ret;
}
