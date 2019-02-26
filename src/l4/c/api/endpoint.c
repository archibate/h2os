#include <l4/api/endpoint.h>
#include <l4/generic/endpoint.h>
#include <l4/generic/sched.h>
#include <l4/enum/errno.h>
#include <l4/enum/filedes.h>
#include <l4/generic/sched.h>
#include <l4/enum/rflags.h>
#include <l4/misc/bug.h>//
#include <l4/misc/panic.h>//

static int fd_verify(l4fd_t fd, unsigned int access)
{
	if (fd >= MAX_FDS)
		return -ENFILE;

	struct fd_entry *fde = &current->fds[fd];

	if (fde->ptr == NULL)
		return -ENFILE;

	BUG_ON(fde->rtype != RTYPE_ENDPOINT);//

	if (~fde->flags & access)
		return -EPERM;

	return 0;
}

static struct endpoint *fd_get_ep(l4fd_t fd)
{
	return (struct endpoint *) current->fds[fd].ptr;
}

int sys_nbsend(l4fd_t fd)
{
	int err = fd_verify(fd, R_WRONLY);
	if (err < 0)
		return err;
	struct endpoint *ep = fd_get_ep(fd);
	endpoint_call(ep, current, false, false);
	return 0;
}

int sys_send(l4fd_t fd)
{
	int err = fd_verify(fd, R_WRONLY);
	if (err < 0)
		return err;
	struct endpoint *ep = fd_get_ep(fd);
	endpoint_call(ep, current, true, false);
	return 0;
}

int sys_call(l4fd_t fd)
{
	int err = fd_verify(fd, R_WRONLY);
	if (err < 0)
		return err;
	struct endpoint *ep = fd_get_ep(fd);
	endpoint_call(ep, current, true, true);
	return 0;
}

int sys_recv(l4fd_t fd)
{
	int err = fd_verify(fd, R_RDONLY);
	if (err < 0)
		return err;
	struct endpoint *ep = fd_get_ep(fd);
	endpoint_wait(ep, current);
	return 0;
}
