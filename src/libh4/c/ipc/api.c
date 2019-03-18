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
	return r;
}

int ipc_recv(int fd)
{
	int r = sys_recv(fd);
	ipc_rewind();
	if (r < 0) *(long*)ipc_buffer = r;
	return r;
}

int ipc_reply(void)
{
	return sys_reply();
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
