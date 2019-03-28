#include <h4/file/api.h>
#include <h4/file/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <compiler.h>
#include <errno.h>

static ssize_t frag_pread(int fd, void *buf, size_t len, off_t off)
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

#ifdef FRAG_SIZE
#include <numtools.h>
//#include <printk.h>//
ssize_t pread(int fd, void *buf, size_t len, off_t off)
{
	size_t lened = 0;
	//printk("!!len=%d", len);//
	while (lened < len) {
		//printk("!!lened=%d", lened);//
		size_t m = len - lened;
		CLMAX(m, FRAG_SIZE);
		//printk("!!m=%d", m);//
		ssize_t ret = frag_pread(fd, buf, m, off);
		//printk("!!ret=%d", ret);//
		if (ret < 0)
			return !lened ? ret : lened;
		lened += ret;
		off += ret;
		if (ret != m)
			break;
	}
	//printk("!!lened=%d", lened);//
	return lened;
}
#else
ssize_t pread(int fd, void *buf, size_t len, off_t off)
{
	return frag_pread(fd, buf, len, off);
}
#endif
