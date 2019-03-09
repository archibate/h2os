#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/sysnr.h>
#include <memory.h>
#include <errno.h>
#include <bug.h>
#include <printk.h>

static int hello;

void hello_init(void)
{
	hello = ipc_open(SVID_HELLO, IPC_CREAT | IPC_SERVER);
	BUG_ON(hello < 0);
}

char data[] = "Hello, World!\n";

#define CLMAX(x, max) do { if ((x) >= (max)) (x) = (max); } while (0)

ssize_t hello_pread(void *badge, void *buf, size_t len, off_t off)
{
	CLMAX(len, sizeof(data) - off);
	memcpy(buf, data + off, len);
	//for (int i = 0; i < 10009000; i++);
	//printk("hello: pread %s %d %d", buf, len, off);
	return len;
}

ssize_t hello_pwrite(void *badge, const void *buf, size_t len, off_t off)
{
	CLMAX(len, sizeof(data) - off);
	memcpy(data + off, buf, len);
	return len;
}

void hello_serve_ipc(void)
{
	ipc_recv(hello);
	unsigned int nr = ipc_getw();
	switch (nr) {

	case _FILE_pread:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		ipc_seek_set(sizeof(ssize_t));
		void *buf = ipc_getbuf(&len);
		void *badge = ipc_getbadge();
		ssize_t ret = hello_pread(badge, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		const void *buf = ipc_getbuf(&len);
		void *badge = ipc_getbadge();
		ssize_t ret = hello_pwrite(badge, buf, len, off);
		ipc_rewindw(ret);
	} break;

	default:
		ipc_putw(-ENOTSUP);
	}
	ipc_reply();
}

int main(void)
{
	hello_init();
	while (1) {
		hello_serve_ipc();
	}
}
