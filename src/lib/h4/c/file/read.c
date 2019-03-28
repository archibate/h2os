#include <h4/file/api.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <compiler.h>
#include <errno.h>

static ssize_t frag_read(int fd, void *buf, size_t len)
{
	ipc_rewindw(_FILE_read);
	ipc_putw(len);
	ipc_call(fd);
	ssize_t ret = ipc_getw();
	if (likely(ret > 0)) {
		if (unlikely(ret > len))
			return -EBADSVC;
		ipc_read(buf, ret);
	}
	return ret;
}

#ifdef FRAG_SIZE
#include <numtools.h>
ssize_t read(int fd, void *buf, size_t len)
{
	size_t lened = 0;
	while (lened < len) {
		size_t m = len;
		CLMAX(m, FRAG_SIZE);
		ssize_t ret = frag_read(fd, buf, m);
		if (ret < 0)
			return !lened ? ret : lened;
		lened += ret;
		if (ret != m)
			break;
	}
	return lened;
}
#else
ssize_t read(int fd, void *buf, size_t len)
{
	return frag_read(fd, buf, len);
}
#endif
