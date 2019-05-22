#include <l4/generic/mman.h>

void mm_copy_fds(struct mm *dst_mm, struct mm *src_mm)
{
	dst_mm->fdtop = src_mm->fdtop;
	memcpy(dst_mm->fds, src_mm->fds, sizeof(dst_mm->fds));
	int i;
	for (i = 0; i < array_sizeof(dst_mm->fds); i++) {
		struct endpoint *ep = dst_mm->fds[i].ep;
		if (ep) ep->refcount++;
	}
}
