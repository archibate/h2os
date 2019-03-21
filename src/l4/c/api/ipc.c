#include <l4/api/ipc.h>
#include <l4/generic/endpoint.h>
#include <l4/generic/sched.h>
#include <l4/enum/errno.h>
#include <l4/enum/filedes.h>
#include <l4/generic/sched.h>
#include <l4/enum/rflags.h>
#include <l4/generic/msginfo.h>
#include <l4/misc/bug.h>//
#include <l4/misc/panic.h>//

static int fd_verify(l4fd_t fd, bool isrecv)
{
	if (fd >= MAX_FDS)
		return -ENFILE;

	struct fd_entry *fde = &current->fds[fd];

	if (fde->ptr == NULL)
		return -EBADF;

	BUG_ON(fde->rtype != RTYPE_ENDPOINT);//

	/*if (~fde->isrecv != isrecv)
		return -EPERM;*/

	return 0;
}

#define get_fde(fd) (current->fds[fd])
static inline struct endpoint *fd_get_ep(l4fd_t fd)
{
	return (struct endpoint *) get_fde(fd).ptr;
}

#define SWAP(x, y) do { typeof(y) t = y; y = x; x = t; } while (0)

static int do_sys_send(l4fd_t fd, bool block, bool recv, int phase)
{
	int err = fd_verify(fd, false);
	if (err < 0)
		return err;
	struct endpoint *ep = fd_get_ep(fd);
	struct ktcb *target = endpoint_call(ep, current, block, recv);
	struct msginfo *mip = &get_fde(fd).msginfo;
	//current->ipcphase = phase ? phase : fd_entry(fd)->phase;
	//fd_entry(fd)->msginfo.phase = phase;
	if (target != NULL) {
		//target->msginfo.phase = fd_entry(fd)->phase;
		target->prplmip = mip;
		msginfo_copy(&target->msginfo, mip);
		//printk("do_sys_send: %d", *(int*)current->ipcbuf);
		SWAP(current->ipcbuf, target->ipcbuf);
	} else {
		current->psndmip = mip;
		//current->msginfo = *current->replymsi;
	}
	return 0;
}

#if 0//{{{
sl4fd_t sys_callfdat(l4fd_t fd, sl4fd_t dirfd)
{
	BUG_ON(dirfd != -1);

	int err = fd_verify(fd, R_WRONLY);
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

int sys_recv(void)
{
	struct endpoint *ep = &current->ep;//TOD
	struct ktcb *target = endpoint_wait(ep, current);
	if (target != NULL) {
		current->prplmip = target->psndmip;
		msginfo_copy(&current->msginfo, target->psndmip);
		//printk("%p", target);
//#include <l4/system/kbase.h>
		//*(int*)KernIPCBuffer = 12345;
		//printk("sys_recv: %d", *(int*)target->ipcbuf);
		SWAP(current->ipcbuf, target->ipcbuf);
		//printk("sys_recv to curr: %d", *(int*)current->ipcbuf);
	}
	return 0;
}

int sys_reply(uintptr_t badge, uintptr_t offset)
{
	//printk("sys_reply: badge=%p", badge);
	if (current->prplmip != NULL) {
		current->prplmip->badge = badge;
		current->prplmip->offset = offset;
	}
	current->prplmip = NULL;
	struct ktcb *target = endpoint_reply(current); // T,ep
	if (target != NULL) {
		//printk("sys_reply: %d", *(int*)current->ipcbuf);
		SWAP(current->ipcbuf, target->ipcbuf);
	}
	return target == NULL ? 0 : 1;
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
