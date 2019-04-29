#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/mm/sysnr.h>
#include <h4/servers.h>
#include <errno.h>
#include <case.h>
#include <printk.h>
#include <bug.h>

struct mm
{
	uintptr_t ebss;
};

uintptr_t do_sbrk(ptrdiff_t incptr)
{
	if (incptr > 0) {
		printk("heap grow %#x", incptr);
		curr->ebss += incptr;
	} else if (incptr < 0) {
		incptr = -incptr;
		printk("heap shrink %#x", incptr);
		curr->ebss -= incptr;
	}
	return curr->ebss;
}

struct mm *new_mm(uintptr_t ebss)
{
	struct mm *mm = malloc(sizeof(struct mm));
	mm->ebss = ebss;
	return mm;
}

const int libh4_serve_id = SVID_MM;

int main(void)
{
	while (1) {
		ipc_recv();
		int nr = ipc_getw();

		struct mm *mm = (void*)ipc_getbadge();
		if (mm == NULL) {
			switch (nr) {
			CASE(_MM_newinit) {
				uintptr_t ebss = ipc_getw();
				mm = mm_new(ebss);
				ipc_setbadge((uintptr_t)mm);
				ipc_rewindw(mm == NULL ? -errno : 0);
			}
			DEFAULT {
				ipc_rewindw(-ENOTSUP);
			}
			}
		} else {
			switch (nr) {
			CASE(_MM_sbrk) {
				ptrdiff_t incptr = ipc_getw();
				uintptr_t ret = do_sbrk(mm, incptr);
				ipc_rewindw(ret);
				ipc_putw(ret >= 0 ? mm->ebss : 0);
			}
			DEFAULT {
				ipc_rewindw(-ENOTSUP);
			}
			}
		}
		ipc_reply();
	}
}
