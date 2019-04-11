#include <h4/file.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>

static ssize_t frag_pwrite(int fd, const void *buf, size_t len, off_t off)
{
	ipc_rewindw(_FILE_pwrite);
	ipc_putw(len);
	ipc_putw(off);
	ipc_write(buf, len);
	ipc_call(fd);
	return ipc_getw();
}

#ifdef FRAG_SIZE
#include <numtools.h>
ssize_t pwrite(int fd, const void *buf, size_t len, off_t off)
{
	size_t lened = 0;
	while (lened < len) {
		size_t m = len;
		CLMAX(m, FRAG_SIZE);
		ssize_t ret = frag_pwrite(fd, buf, m, off);
		if (ret < 0)
			return !lened ? ret : lened;
		lened += ret;
		off += ret;
		if (ret != m)
			break;
	}
	return lened;
}
#else
ssize_t pwrite(int fd, const void *buf, size_t len, off_t off)
{
	return frag_pwrite(fd, buf, len, off);
}
#endif
