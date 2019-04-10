#include <l4/api/open.h>
#include <l4/generic/idget.h>
#include <l4/generic/gfopen.h>
#include <l4/object/thread.h>
#include <l4/enum/rtype.h>
#include <l4/enum/errno.h>

sl4fd_t sys_connect(l4id_t tid)
{
	struct ktcb *tcb = id_get_thread(tid);
	if (!tcb) return -ESRCH;

	return gf_open(&tcb->ep, RTYPE_ENDPOINT);
}

int sys_close(l4fd_t fd)
{
	return gf_close(fd);
}

int sys_dup(int fd)
{
	return gf_dup(fd);
}

int sys_dup2(int fd, int dirfd)
{
	return gf_dup2(fd, dirfd);
}
