#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/sysnr.h>
#include <memory.h>
#include <errno.h>
#include <stddef.h>
#include <bug.h>
#include <printk.h>
//#define ID_OBJECT_CONTENTS off_t off;
#include <liballoc.h>
#include <malloc.h>

struct hello_file
{
	void *data;
	off_t off;
	size_t size;
};

#define CLMAX(x, max) do { if ((x) > (max)) (x) = (max); } while (0)
#define CLMIN(x, min) do { if ((x) < (min)) (x) = (min); } while (0)

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

ssize_t hello_read(struct hello_file *fp, void *buf, size_t len)
{
	ssize_t ret = hello_pread(fp, buf, len, fp->off);
	if (ret > 0)
		fp->off += ret;
	return ret;
}

ssize_t hello_write(struct hello_file *fp, const void *buf, size_t len)
{
	ssize_t ret = hello_pwrite(fp, buf, len, fp->off);
	if (ret > 0)
		fp->off += ret;
	return ret;
}

off_t hello_lseek(struct hello_file *fp, off_t off, int whence)
{
	//printk("hello_lseek(%d, %d)", off, whence);
	switch (whence) {
	case 0: break;
	case 1: off += fp->off; break;
	case 2: off = fp->size + off; break;
	default: return -ENOTSUP;
	};
	CLMIN(off, 0);
	CLMAX(off, fp->size);
	fp->off = off;
	return off;
}

struct hello_file *hello_new_open(unsigned int flags)
{
	/*struct hello_file *fp = malloc(sizeof(struct hello_file));
	fp->id = conn;
	BUG_ON(NULL != idmap_add(&ftab, fp));*/
	struct hello_file *fp = malloc(sizeof(struct hello_file));
	fp->off = 0;
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
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_pread(fp, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_pwrite(fp, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		size_t len = ipc_getw();
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_read(fp, buf, len);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_write(fp, buf, len);
		ipc_rewindw(ret);
	} break;

	case _FILE_lseek:
	{
		off_t off = ipc_getw();
		int whence = ipc_getw();
		off_t ret = hello_lseek(fp, off, whence);
		ipc_rewindw(ret);
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}

static char buffer[4096*4];

int main(void)
{
	liballoc_set_memory(buffer, sizeof(buffer));

	int conn = ipc_open(SVID_HELLO, IPC_CREAT|IPC_SERVER);
	BUG_ON(conn < 0);
	while (1) {
		ipc_recv(conn);
		struct hello_file *fp = (void*)ipc_getbadge();
		if (ipc_isclose()) {
			hello_close(fp);
		} else {
			if (!fp) {
				unsigned int flags = 0;//TOD
				fp = hello_new_open(flags);
				ipc_setbadge((uintptr_t)fp);
			}
			hello_serve_ipc(fp);
		}
	}
	ipc_close(conn);
}
