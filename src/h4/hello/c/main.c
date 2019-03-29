#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/sysnr.h>
#include <memory.h>
#include <errno.h>
#include <stddef.h>
#include <bug.h>
#include <printk.h>
#include <c4/liballoc.h>
#include <malloc.h>
#include <numtools.h>

struct hello_file
{
	void *data;
	size_t size;
};

ssize_t hello_pread(struct hello_file *fp, void *buf, size_t len, off_t off)
{
	CLMAX(len, fp->size - off);
	memcpy(buf, fp->data + off, len);
	return len;
}

ssize_t hello_pwrite(struct hello_file *fp, const void *buf, size_t len, off_t off)
{
	CLMAX(len, fp->size - off);
	memcpy(fp->data + off, buf, len);
	return len;
}

off_t hello_lseek(struct hello_file *fp, off_t now_off, off_t off, int whence)
{
	switch (whence) {
	case 0: break;
	case 1: off += now_off; break;
	case 2: off = fp->size + off; break;
	default: return -ENOTSUP;
	};
	CLMIN(off, 0);
	CLMAX(off, fp->size);
	return off;
}

struct hello_file *hello_new_open(unsigned int flags)
{
	/*struct hello_file *fp = malloc(sizeof(struct hello_file));
	fp->id = conn;
	BUG_ON(NULL != idmap_add(&ftab, fp));*/
	struct hello_file *fp = malloc(sizeof(struct hello_file));
	static char data[] = "Hello, World!\n";
	fp->data = &data;
	fp->size = sizeof(data)-1;
	BUG_ON(fp == NULL);
	return fp;
}

void hello_close(struct hello_file *fp)
{
	free(fp);
}

void hello_serve_ipc(struct hello_file *fp)
{
	unsigned int nr = ipc_getw();
	switch (nr) {

	case _FILE_pread:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("hello_pread(%d, %d)", len, off);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_pread(fp, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("hello_pwrite(%d, %d)", len, off);
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_pwrite(fp, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		size_t len = ipc_getw();
		//printk("hello_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = hello_pread(fp, buf, len, off);
		if (ret > 0)
			off += ret;
		ipc_setoffset(off);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		//printk("hello_write(%d)", len);
		const void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = hello_pwrite(fp, buf, len, off);
		if (ret > 0)
			off += ret;
		ipc_setoffset(off);
		ipc_rewindw(ret);
	} break;

	case _FILE_lseek:
	{
		off_t off = ipc_getw();
		int whence = ipc_getw();
		off_t now_off = ipc_getoffset();
		off_t ret = hello_lseek(fp, now_off, off, whence);
		if (ret >= 0)
			ipc_setoffset(ret);
		ipc_rewindw(ret);
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}

int main(void)
{
	static char buffer[4096*4];
	liballoc_set_memory(buffer, sizeof(buffer));

	ipc_serve(SVID_HELLO);
	while (1) {
		ipc_recv();
		struct hello_file *fp = (void*)ipc_getbadge();
		if (ipc_isclose()) {
			hello_close(fp);
		} else {
			if (!fp) {
				unsigned int flags = 0;//TOD
				fp = hello_new_open(flags);
				//printk("fp=%p", fp);
				ipc_setbadge((uintptr_t)fp);
			}
			hello_serve_ipc(fp);
		}
	}
}
