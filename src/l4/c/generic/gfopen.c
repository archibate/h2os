#include <l4/generic/gfopen.h>
#include <l4/generic/sched.h>
#include <l4/enum/errno.h>
#include <l4/misc/bug.h>
#include <memory.h>

static int alloc_fd(void)
{
	int i;
	for (i = current->fdtop; i < MAX_FDS; i++)
		if (current->fds[i].ptr == NULL)
			goto found;
	return -1;
found:
	current->fdtop = i + 1;
	return i;
}

static void free_fd(int fd)
{
	if (fd < current->fdtop)
		current->fdtop = fd;

	memset(&current->fds[fd], 0, sizeof(struct fd_entry));
}

sl4fd_t gf_open(void *p, unsigned int rtype)
{
	BUG_ON(p == NULL);

	int fd = alloc_fd();
	if (fd < 0)
		return -EMFILE;

	struct fd_entry *fde = &current->fds[fd];

	fde->rtype = rtype;
	fde->ptr = p;

	return fd;
}

int gf_close(l4fd_t fd)
{
	if (fd >= MAX_FDS)
		return -ENFILE;

	free_fd(fd);

	struct fd_entry *fde = &current->fds[fd];

	return 0;
}

sl4fd_t gf_dup(l4fd_t fd)
{
	if (fd >= MAX_FDS)
		return -ENFILE;

	struct fd_entry *fde = &current->fds[fd];
	if (fde->ptr == NULL)
		return -EBADF;

	int nfd = alloc_fd();
	if (nfd < 0)
		return -EMFILE;

	struct fd_entry *nfde = &current->fds[nfd];

	memcpy(nfde, fde, sizeof(struct fd_entry));

	return nfd;
}

int gf_dup2(l4fd_t fd, l4fd_t dirfd)
{
	if (fd >= MAX_FDS)
		return -ENFILE;

	struct fd_entry *fde = &current->fds[fd];
	if (fde->ptr == NULL)
		return -EBADF;

	struct fd_entry *nfde = &current->fds[dirfd];
	if (nfde->ptr != NULL)
		return -ECLOSAT;

	memcpy(nfde, fde, sizeof(struct fd_entry));

	return 0;
}
