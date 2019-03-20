#include <h4/file/api.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <compiler.h>
#include <errno.h>

ssize_t pread(int fd, void *buf, size_t len, off_t off)
{
	ipc_rewindw(_FILE_pread);
	ipc_putw(len);
	ipc_putw(off);
	ipc_call(fd);
	ssize_t ret = ipc_getw();
	if (likely(ret > 0)) {
		if (unlikely(ret > len))
			return -EBADSVC;
		ipc_read(buf, ret);
	}
	return ret;
}
