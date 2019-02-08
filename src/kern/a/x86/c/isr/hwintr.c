#include <k/x86/idt.h>
#include <k/asm/iframe.h>
#include <k/a/dumpuser.h>
#include <k/kstack.h>
#include <k/printk.h>
#include <k/panic.h>
#include <assert.h>
#include <x86/cregs.h>

void hwirq(uint irq);
void hwsysintr(void);
//void hwpgfault(ulong vaddr, uint errcd);

void hwintr(ulong *iframe)
{
	uint nr = iframe[IFrame_IntrNum];

	if (nr >= IntrIRQ0 && nr < IntrSyscall) {
		hwirq(nr - IntrIRQ0);
		return;
	} else if (nr == IntrSyscall) {
		assert(iframe == kIFrame);
		hwsysintr();
		return;
	} else if (nr > IntrSyscall) {
		assert(iframe == kIFrame);
		//dprintk("int %#x", nr);
		//dumpuser();
		return;
	} else switch (nr) {
	case ExceptionPageFault:
		panic("#PF from %#04x:%p at %#p (%d)",
				iframe[IFrame_CS], iframe[IFrame_EIP],
				getcr2(), iframe[IFrame_ErrorCode]);
		return;
	}

	panic("Undefined Interrupt Number %d (%#x)", nr, nr);
}
