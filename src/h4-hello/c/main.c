#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/sysnr.h>
#include <memory.h>
#include <errno.h>
#include <stddef.h>
#include <bug.h>
#include <printk.h>
#define ID_OBJECT_CONTENTS off_t off;
#include <idmap.h>
#include <liballoc.h>
#include <malloc.h>

struct idmap ftab;

char data[] = "Hello, World!\n";

#define CLMAX(x, max) do { if ((x) >= (max)) (x) = (max); } while (0)

ssize_t hello_pread(void *buf, size_t len, off_t off)
{
	CLMAX(len, sizeof(data) - off);
	memcpy(buf, data + off, len);
	return len;
}

ssize_t hello_pwrite(const void *buf, size_t len, off_t off)
{
	CLMAX(len, sizeof(data) - off);
	memcpy(data + off, buf, len);
	return len;
}

#define hello_file idobj
void hello_on_open(int conn)
{
	struct hello_file *fp = malloc(sizeof(struct hello_file));
	fp->id = conn;
	BUG_ON(NULL != idmap_add(&ftab, fp));
}

void hello_on_close(int conn)
{
	BUG_ON(NULL == idmap_del(&ftab, conn));
}

bool hello_serve_ipc(int conn)
{
	ipc_recv(conn);
	unsigned int nr = ipc_getw();
	switch (nr) {

	case _FILE_pread:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_pread(buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_pwrite(buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		size_t len = ipc_getw();
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		off_t off = idmap_get(&ftab, conn)->off;
		ssize_t ret = hello_pread(buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		const void *buf = ipc_getbuf(&len);
		off_t off = idmap_get(&ftab, conn)->off;
		ssize_t ret = hello_pwrite(buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_close:
	{
		ipc_replyw(0);
		return false;
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
	return true;
}

static char buffer[4096*2];

int main(void)
{
	liballoc_set_memory(buffer, sizeof(buffer));

	int conn = ipc_open(SVID_HELLO, IPC_CREAT|IPC_SERVER);
	BUG_ON(conn < 0);
	hello_on_open(conn);
	while (1) {
		hello_serve_ipc(conn);
	}
	hello_on_close(conn);
	ipc_close(conn);
}
