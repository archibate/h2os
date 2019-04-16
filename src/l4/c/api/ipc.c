#include <l4/api/ipc.h>
#include <l4/api/mmap.h>
#include <l4/generic/endpoint.h>
#include <l4/generic/sched.h>
#include <l4/enum/errno.h>
#include <l4/enum/filedes.h>
#include <l4/generic/sched.h>
#include <l4/enum/rflags.h>
#include <l4/generic/msginfo.h>
#include <l4/misc/bug.h>//
#include <l4/misc/panic.h>//
// for sys_mmap!!!!!!!!
#include <l4/generic/mman.h>
#include <memory.h>//??gf_copy

#define get_fde(fd) (current->fds[fd])

static int fd_verify(l4fd_t fd)
{
	if (fd >= MAX_FDS)
		return -ENFILE;

	struct fd_entry *fde = &get_fde(fd);

	if (fde->ptr == NULL)
		return -EBADF;

	BUG_ON(fde->rtype != RTYPE_ENDPOINT);//

	/*if (~fde->isrecv != isrecv)
		return -EPERM;*/

	return 0;
}

static inline struct endpoint *fd_get_ep(l4fd_t fd)
{
	return (struct endpoint *) get_fde(fd).ptr;
}

static int do_sys_send(l4fd_t fd, bool block, bool recv, int phase)
{
	int err = fd_verify(fd);
	if (err < 0)
		return err;

	return endp_call(&get_fde(fd), block, recv, phase);
}

#if 0//{{{
sl4fd_t sys_callfdat(l4fd_t fd, sl4fd_t dirfd)
{
	BUG_ON(dirfd != -1);

	int err = fd_verify(fd);
	if (err < 0)
		return err;
	struct endpoint *ep = fd_get_ep(fd);
	struct ktcb *target = endpoint_call(ep, current, block, recv);
	if (target != NULL) {
		//printk("do_sys_send: %d", *(int*)current->ipcbuf);
		SWAP(current->ipcbuf, target->ipcbuf);
	}
	l4fd_t fd = gf_open(RTYPE_ENDPOINT, R_WRONLY);
	return fd;
}
#endif//}}}
#if 0//{{{
int sys_connect(l4id_t id, unsigned int flags)
{
	int fd = sys_rt_open(id, RTYPE_ENDPOINT, flags);
	if (fd < 0)
		return fd;
	int ret = do_sys_send(fd, true, true, 1);
	if (ret < 0) {
		sys_rt_close(fd);
		return ret;
	}
	return fd;
}
#endif//}}}

int do_sys_recv(bool block)
{
	struct endpoint *ep = &current->ep;//TOD
	return endp_recv(ep, block);
}

int sys_recv(void)
{
	return do_sys_recv(true);
}

int sys_poll(void)
{
	return do_sys_recv(false);
}

int sys_reply(uintptr_t badge, uintptr_t offset)
{
	return endp_reply(badge, offset);
}

int sys_nbsend(l4fd_t fd)
{
	return do_sys_send(fd, false, false, 0);
}

int sys_send(l4fd_t fd)
{
	return do_sys_send(fd, true, false, 0);
}

int sys_call(l4fd_t fd)
{
	return do_sys_send(fd, true, true, 0);
}

int sys_mmap(l4fd_t fd, void *p, size_t size, unsigned int mattr)
{
	int err = fd_verify(fd);
	if (err < 0)
		return err;

	struct fd_entry *fde = &get_fde(fd);

	struct mregion *mreg = mm_new(current->mm, (word_t)p, (word_t)p + size);
	if (mreg == NULL)
		return -EFAULT;

	memcpy(&mreg->fde, fde, sizeof(struct fd_entry));
	return 0;
}
