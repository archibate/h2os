#include <h4/proc/sysnr.h>
#include <h4/filedes.h>
#include <liballoc.h>
#include <bug.h>
#ifndef _HARDHEAP_SIZE
#define _HARDHEAP_SIZE (4096*2)
#endif

static int proc_rootclinit(void *ebss)
{
	ipc_rewindw(_PROC_rootclinit);
	ipc_putw((uintptr_t)ebss);
	ipc_call(SVFD_PROC);
	return ipc_getw();
}

void __rootclinit(void)
{
	extern char _ebss[0];
	BUG_ON(proc_rootclinit(&_ebss) < 0);
}
