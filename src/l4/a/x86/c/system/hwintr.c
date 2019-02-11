#include <l4/system/asm/idt.h>
#include <l4/object/iframe.h>
#include <l4/misc/dumpuser.h>
#include <l4/system/kstack.h>
#include <l4/misc/printk.h>
#include <l4/misc/panic.h>
#include <l4/misc/assert.h>
#include <l4/machine/asm/cregs.h>

void hwirq(uint irq);
void hwsysintr(void);
//void hwpgfault(ulong vaddr, uint errcd);

void _FASTCALL hwintr(struct iframe *iframe)
{
	uint nr = iframe->intr_num;

	if (nr >= INTR_IRQ0 && nr < INTR_SWI0) {
		hwirq(nr - INTR_IRQ0);
		return;
	} else if (nr == INTR_SWI0) {
		assert(iframe == &kIFrame);
		hwsysintr();
		return;
	} else if (nr > INTR_SWI0) {
		assert(iframe == &kIFrame);
#ifdef CONFIG_LOG_EXTRA_SWI
		printk("int %#x", nr);
		//dumpuser();
#endif
		return;
	} else switch (nr) {
	case ExceptionPageFault:
		panic("#PF from %#04x:%p at %#p (%d)",
				iframe->cs, iframe->pc,
				getcr2(), iframe->error_code);
	case ExceptionGeneralProtection:
		panic("#GP from %#04x:%p of instruction %02X",
				iframe->cs, iframe->pc,
				*(volatile uchar*)iframe->pc
				);
	case ExceptionInvaidOpcode:
		panic("#UD from %#04x:%p of instruction %02X",
				iframe->cs, iframe->pc,
				*(volatile uchar*)iframe->pc
				);
	case ExceptionBreakPoint:
		printk("Int3 from %#04x:%p (EAX=%p ESP=%p)",
				iframe->cs, iframe->pc,
				iframe->eax, iframe->sp
				);
		return;
	default:
		panic("Undefined Interrupt Number %d (%#x)", nr, nr);
	}

}
