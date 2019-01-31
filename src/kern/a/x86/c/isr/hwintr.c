#include <k/x86/idt.h>
#include <k/asm/iframe.h>
#include <k/kstack.h>
#include <k/panic.h>
#include <assert.h>
#include <x86/cregs.h>

void hwirq(uint irq);
void hwsysintr(void);
void hwpgfault(ulong vaddr, uint errcd);

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
	} else switch (nr) {
	case ExceptionPageFault:
		hwpgfault(getcr3(), iframe[IFrame_ErrorCode]);
		return;
	}

	panic("Undefined Interrupt Number %d (%#x)", nr, nr);
}

void hwpgfault(ulong vaddr, uint errcd)
{
	panic("Undefined Page Fault at vaddr=%#p, errcd=%#x", vaddr, errcd);
}

void hwirq(uint irq)
{
	printk("Undefined IRQ %d (%#x)", irq, irq);
}
