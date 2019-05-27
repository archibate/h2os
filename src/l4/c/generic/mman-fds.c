#include <l4/generic/mman.h>
#include <l4/enum/errno.h>

int mm_copy_fd(struct mm *dst_mm, struct mm *src_mm, l4fd_t fdst, l4fd_t fsrc)
{
	if (fsrc >= MAX_FDS || fdst >= MAX_FDS)
		return -ENFILE;
	struct fd_entry *src = &src_mm->fds[fsrc];
	struct fd_entry *dst = &dst_mm->fds[fdst];
	if (!src->ep) return -EBADF;
	if (dst->ep) return -ECLOSAT;
	src->ep->refcount++;
	printk("mm_copy_fd: %p/%d->%p/%d", src_mm, fsrc, dst_mm, fdst);
	memcpy(dst, src, sizeof(struct fd_entry));
	BUG_ON(dst->ep == NULL);
	//if (fdst > dst_mm->fdtop)
		//dst_mm->fdtop = fdst;
	return 0;
}
