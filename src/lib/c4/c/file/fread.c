#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <numtools.h>

ssize_t fread(void *buf, size_t size, size_t nmemb, FILE *fp)
{
	if (fp->wr)
		return -EPERM;

	size_t len = size * nmemb;
	size_t m = fp->n;
	ssize_t r;
	CLMAX(m, len);
	memcpy(buf, fp->p, m);
	fp->p += m;
	fp->n -= m;
	if (m == len) {
		//printk("LE!!!!");
		return nmemb;
	}
	buf += m;
	len -= m;

	if (len > BUFSIZ) {
		//printk("BB!!!!");
		r = read(fp->fd, buf, len);
		//printk("BB %d/%d (%02X)", r, len, *(unsigned char*)buf);
		if (r < 0) {
			errno = fp->err = -r;
			return r;
		} else if (r == 0) {
			fp->eof = 1;
		}
		r += m;

	} else {
		//printk("RD!!!!");
		r = read(fp->fd, fp->b, BUFSIZ);
		if (r < 0) {
			errno = fp->err = -r;
			return r;
		} else if (r == 0) {
			fp->eof = 1;
		}
		CLMAX(len, r);
		memcpy(buf, fp->b, len);
		fp->p = fp->b + len;
		fp->n = r - len;
		r = len + m;
	}

	return r / size;
}
