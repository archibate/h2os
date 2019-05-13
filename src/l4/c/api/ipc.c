#include <l4/api/ipc.h>
#include <l4/api/mmap.h>
#include <l4/enum/mmprot.h>
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
#include <l4/lib/lohitools.h>

static struct fd_entry *g_fde;

static int fd_verify(l4fd_t fd)
{
	if (fd >= MAX_FDS)
		return -ENFILE;

	g_fde = ids_get(&current->fds);

	if (g_fde == NULL)
		return -EBADF;

	BUG_ON(g_fde->ep == NULL);

	return 0;
}

static int do_sys_send(l4fd_t fd, bool block, bool recv, int phase)
{
	int err = fd_verify(fd);
	if (err < 0)
		return err;

	return endp_call(g_fde, block, recv, phase);
}

int do_sys_recv(bool block)
{
	struct endpoint *ep = &current->ep;
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

int sys_reply(uintptr_t badge, uintptr_t offset, uintptr_t type)
{
	return endp_reply(badge, offset, type);
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

// https://baike.so.com/doc/6784320-7000923.html
int sys_mmap(l4id_t mmc, l4fd_t fd, void *p, size_t size)
{
	int err = fd_verify(fd);
	if (err < 0)
		return err;

	unsigned int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
	unsigned int flags = 0;

	return softfault_mmap(mmc, g_fde, (word_t)p, size, flags, prot);
}

int sys_msync(l4id_t mmc, void *p, size_t size)
{
	return softfault_msync(mmc, (word_t)p, size);
}

int sys_munmap(l4id_t mmc, void *p, size_t size)
{
	return softfault_munmap(mmc, (word_t)p, size);
}

#include <l4/generic/softfault.h>//
int sys_test_fault(l4id_t mmc, void *p, unsigned int errcd)
{
	softfault_callback(mmc, (word_t)p, errcd);
	return 0;
}
