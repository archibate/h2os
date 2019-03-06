#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/fs/sysnr.h>
#include <errno.h>
#include <bug.h>

static int fss;

void fss_init(void)
{
	fss = ipc_open(SVID_HELLO, IPC_CREAT | IPC_SERVER);
	BUG_ON(fss < 0);
}

char data[] = "Hello, World!\n";

#define CLMAX(x, max) do { if ((x) >= (max)) (x) = (max); } while (0)

ssize_t fss_pread(void *badge, size_t off, void *buf, size_t len)
{
	CLMAX(len, sizeof(data) - off);
	memcpy(buf, data + off, len);
	return len;
}

ssize_t fss_pwrite(void *badge, size_t off, const void *buf, size_t len)
{
	CLMAX(len, sizeof(data) - off);
	memcpy(data + off, buf, len);
	return len;
}

void fss_serve_ipc(void)
{
	ipc_recv(fss);
	unsigned int nr = ipc_getw();
	switch (nr) {

	case _FS_pread:
	{
		size_t off = ipc_getw();
		size_t len = ipc_getw();
		void *buf = ipc_getbuf(&len);
		void *badge = ipc_getbadge();
		ssize_t ret = fss_pread(badge, off, buf, len);
		ipc_putw(ret);
	} break;

	case _FS_pwrite:
	{
		size_t off = ipc_getw();
		size_t len = ipc_getw();
		const void *buf = ipc_getbuf(&len);
		void *badge = ipc_getbadge();
		ssize_t ret = fss_pwrite(badge, off, buf, len);
		ipc_putw(ret);
	} break;

	default:
		ipc_putw(-ENOTSUP);
	}
	ipc_reply();
}

int main(void)
{
	fss_init();
	while (1) {
		fss_serve_ipc();
	}
}
