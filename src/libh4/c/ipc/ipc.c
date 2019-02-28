#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <memory.h>

#define IPC_BUFSIZ 4096

static char ipc_buffer[IPC_BUFSIZ];
static size_t ipc_offset;

void *ipc_getbuf(void)
{
	return ipc_buffer + ipc_offset;
}

void ipc_begin(void)
{
	ipc_rewind();
}

int ipc_write(const void *buf, size_t n)
{
	memcpy(ipc_buffer + ipc_offset, buf, n);
	ipc_offset += n;
	return 0;
}

int ipc_read(void *buf, size_t n)
{
	memcpy(buf, ipc_buffer + ipc_offset, n);
	ipc_offset += n;
	return 0;
}

int ipc_seek_set(size_t off)
{
	ipc_offset = off;
	return 0;
}

int ipc_seek_cur(ssize_t off)
{
	ipc_offset += off;
	return 0;
}

void ipc_rewind(void)
{
	ipc_offset = 0;
}

size_t ipc_tell(void)
{
	return ipc_offset;
}
