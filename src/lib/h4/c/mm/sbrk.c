#include <h4/mm.h>
#include <h4/fs.h>
#include <l4/types.h>
#include <l4/api/mmap.h>
#include <l4/machine/mmu/page.h>
#include <errno.h>

static void *curr_brk, *min_brk;

void __mm_init(void *ebss) // should be called from crt_init(ebss)
{
	min_brk = curr_brk = ebss;
}

void *sbrk(ptrdiff_t incptr)
{
	//printk("sbrk(%#x)", incptr);
	void *old_brk = curr_brk;
	int ret;
	if (incptr > 0) {
		int zero = open("/dev/zero", O_RDONLY);
		ret = sys_mmap(0, zero, curr_brk, incptr);
		if (ret >= 0) {
			void *p;
			for (p = curr_brk; p < curr_brk + incptr; p += PageSize)
				BUG_ON(sys_test_fault(0, p, 6) < 0);
		}
		close(zero);
	} else if (incptr < 0) {
		void *new_brk = curr_brk + incptr;
		if (new_brk < min_brk) {
			errno = -EFAULT;
			return (void*)-1;
		}
		ret = sys_munmap(0, new_brk, -incptr);
	}
	if (ret < 0) {
		errno = -ret;
		return (void*)-1;
	}
	curr_brk += incptr;
	return old_brk;
}
