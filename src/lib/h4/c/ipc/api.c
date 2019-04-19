#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/sys/ipcbuf.h>
#include <l4/api/ipc.h>

#if 0
int ipc_connect(key_t key, unsigned int flags)
{
	int r = sys_connect(key, flags);
	ipc_rewind();
	if (r < 0) *(long*)ipc_buffer = r;
	return r;
}
#endif

static uintptr_t ipc_badge;
static uintptr_t ipc_offset;
static uintptr_t ipc_type;
#if 0
static int ipc_fdreply;
static int ipc_replyfd;
#endif

uintptr_t ipc_getbadge(void)
{
	return ipc_badge;
}

void ipc_setbadge(uintptr_t badge)
{
	ipc_badge = badge;
}

uintptr_t ipc_gettype(void)
{
	return ipc_type;
}

void ipc_settype(uintptr_t type)
{
	ipc_type = type;
}

uintptr_t ipc_getoffset(void)
{
	return ipc_offset;
}

void ipc_setoffset(uintptr_t offset)
{
	ipc_offset = offset;
}

bool ipc_isclose(void)
{
	return false;//TOD
}

#if 0//{{{
void ipc_setfdreply(int fd)
{
	ipc_fdreply = fd;
}

int ipc_getreplyfd(void)
{
	return ipc_replyfd;
}
#endif//}}}

int ipc_send(int fd)
{
	int r = sys_send(fd);
	ipc_rewind();
	//if (r < 0) *(long*)ipc_buffer = r;
	return r;
}

int ipc_nbsend(int fd)
{
	int r = sys_nbsend(fd);
	ipc_rewind();
	//if (r < 0) *(long*)ipc_buffer = r;
	return r;
}

int ipc_call(int fd)
{
	int r = sys_call(fd);
	ipc_rewind();
	if (r < 0) *(long*)ipc_buffer = r;
	//ipc_replyfd = ipc_msginfo.rplfd;
	return r;
}

int ipc_recv(void)
{
	int r = sys_recv();
	ipc_rewind();
	if (r < 0) *(long*)ipc_buffer = r;
	else {
		ipc_badge = ipc_msginfo.badge;
		ipc_offset = ipc_msginfo.offset;
		ipc_type = ipc_msginfo.type;
		//ipc_fdreply = -1;
	}
	return r;
}

int ipc_poll(void)
{
	int r = sys_poll();
	ipc_rewind();
	if (r < 0) *(long*)ipc_buffer = r;
	else {
		ipc_badge = ipc_msginfo.badge;
		ipc_offset = ipc_msginfo.offset;
		ipc_type = ipc_msginfo.type;
		//ipc_fdreply = -1;
	}
	return r;
}

int ipc_reply(void)
{
	return sys_reply(ipc_badge, ipc_offset, ipc_type);
}

#if 0//{{{
int ipc_callfdat(int fd, int dirfd)
{
	int r = sys_callfdat(fd, dirfd);
	ipc_rewind();
	if (r < 0) *(long*)ipc_buffer = r;
	return r;
}
#endif//}}}
