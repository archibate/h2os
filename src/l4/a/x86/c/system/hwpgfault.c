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
	}

	sched_enter();
	softfault_callback(vaddr, errcd);
	sched_leave();
}
