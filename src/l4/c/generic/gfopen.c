#include <l4/generic/gfopen.h>
#include <l4/generic/sched.h>
#include <l4/enum/errno.h>
#include <l4/misc/bug.h>

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

sl4fd_t gf_open(void *p, unsigned int rtype, unsigned int flags)
{
	BUG_ON(p == NULL);

	int fd = alloc_fd();
	if (fd < 0)
		return -EMFILE;

	struct fd_entry *fde = &current->fds[fd];

	fde->rtype = rtype;
	fde->flags = flags;
	fde->ptr = p;

	return fd;
}

int gf_close(l4fd_t fd)
{
	if (fd >= MAX_FDS)
		return -ENFILE;

	if (fd < current->fdtop)
		current->fdtop = fd;

	struct fd_entry *fde = &current->fds[fd];

	fde->rtype = 0;
	fde->flags = 0;
	fde->ptr = NULL;

	return 0;
}
