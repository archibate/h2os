#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <l4/api/keier.h>
#include <l4/api/open.h>

int ipc_open(key_t key)
{
	return sys_kopen(key);
}

int ipc_serve(key_t key)
{
	return sys_kserve(key);
}

int ipc_connect(pid_t pid)
{
	return sys_connect(pid);
}

int ipc_close(int fd)
{
	return sys_close(fd);
}

int ipc_dup(int fd)
{
	return sys_dup(fd);
}

int ipc_dup2(int fd, int dirfd)
{
	return sys_dup2(fd, dirfd);
}
