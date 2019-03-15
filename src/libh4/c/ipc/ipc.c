#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/sys/ipcbuf.h>
#include <memory.h>

static size_t ipc_offset;

void *ipc_getbuf(size_t *plen)
{
	void *p = ipc_buffer + ipc_offset;
	if (plen) ipc_offset += *plen;
	return p;
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

int ipc_seek_setw(size_t cnt)
{
	ipc_offset = sizeof(uintptr_t) * cnt;
	return 0;
}

int ipc_seek_curw(ssize_t cnt)
{
	ipc_offset += sizeof(uintptr_t) * cnt;
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
