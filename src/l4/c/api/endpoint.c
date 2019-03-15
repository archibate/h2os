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
		return -EBADF;

	BUG_ON(fde->rtype != RTYPE_ENDPOINT);//

	if (~fde->flags & access)
		return -EPERM;

	return 0;
}

static struct endpoint *fd_get_ep(l4fd_t fd)
{
	return (struct endpoint *) current->fds[fd].ptr;
}

#define SWAP(x, y) do { typeof(y) t = y; y = x; x = t; } while (0)

static int do_sys_send(l4fd_t fd, bool block, bool recv)
{
	int err = fd_verify(fd, R_WRONLY);
	if (err < 0)
		return err;
	struct endpoint *ep = fd_get_ep(fd);
	struct ktcb *target = endpoint_call(ep, current, block, recv);
	if (target != NULL) {
		//printk("do_sys_send: %d", *(int*)current->ipcbuf);
		SWAP(current->ipcbuf, target->ipcbuf);
	}
	return 0;
}

int sys_recv(l4fd_t fd)
{
	int err = fd_verify(fd, R_RDONLY);
	if (err < 0)
		return err;
	struct endpoint *ep = fd_get_ep(fd);
	struct ktcb *target = endpoint_wait(ep, current);
	if (target != NULL) {
		//printk("%p", target);
//#include <l4/system/kbase.h>
		//*(int*)KernIPCBuffer = 12345;
		//printk("sys_recv: %d", *(int*)target->ipcbuf);
		SWAP(current->ipcbuf, target->ipcbuf);
		//printk("sys_recv to curr: %d", *(int*)current->ipcbuf);
	}
	return 0;
}

int sys_reply(void)
{
	//printk("!!!!");
	struct ktcb *target = endpoint_reply(NULL, current); // T,ep
	if (target != NULL) {
		//printk("sys_reply: %d", *(int*)current->ipcbuf);
		SWAP(current->ipcbuf, target->ipcbuf);
	}
	return target == NULL ? 0 : 1;
}

int sys_nbsend(l4fd_t fd)
{
	return do_sys_send(fd, false, false);
}

int sys_send(l4fd_t fd)
{
	return do_sys_send(fd, true, false);
}

int sys_call(l4fd_t fd)
{
	return do_sys_send(fd, true, true);
}
