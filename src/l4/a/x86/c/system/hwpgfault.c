#include <l4/generic/softfault.h>
#include <l4/generic/sched.h>
#include <l4/misc/printk.h>
#include <l4/misc/panic.h>
#include <l4/machine/mmu/page.h>
#include <l4/machine/mmu/pagefault.h>

void hwpgfault(ulong vaddr, uint errcd)
{
	if (errcd & PageFaultErrorCode_Protect) {
		panic("hwpgfault: protection violated %p (%d)", vaddr, errcd);
		// TOD: SIGSEGF
	}

	sched_enter();
	BUG_ON(softfault_callback(0, vaddr, errcd) <= 0);
	sched_leave();
}
