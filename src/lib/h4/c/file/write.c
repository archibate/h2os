#include <h4/file.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

static ssize_t frag_write(int fd, const void *buf, size_t len)
{
	ipc_rewindw(_FILE_write);
	ipc_putw(len);
	ipc_write(buf, len);
	ipc_call(fd);
	return ipc_getw();
}

#ifdef FRAG_SIZE
#include <numtools.h>
ssize_t write(int fd, const void *buf, size_t len)
{
	size_t lened = 0;
	while (lened < len) {
		size_t m = len;
		CLMAX(m, FRAG_SIZE);
		ssize_t ret = frag_write(fd, buf, m);
		if (ret < 0)
			return !lened ? ret : lened;
		lened += ret;
		if (ret != m)
			break;
	}
	return lened;
}
#else
ssize_t write(int fd, const void *buf, size_t len)
{
	return frag_write(fd, buf, len);
}
#endif
