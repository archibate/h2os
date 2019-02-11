#include <l4/system/asm/idt.h>
#include <l4/system/asm/iframe.h>
#include <l4/misc/dumpuser.h>
#include <l4/system/kstack.h>
#include <l4/misc/printk.h>
#include <l4/misc/panic.h>
#include <l4/misc/assert.h>
#include <l4/machine/asm/cregs.h>

void hwirq(uint irq);
void hwsysintr(void);
//void hwpgfault(ulong vaddr, uint errcd);

void _FASTCALL hwintr(ulong *iframe)
{
	uint nr = iframe[IFrame_IntrNum];

	if (nr >= INTR_IRQ0 && nr < INTR_SWI0) {
		hwirq(nr - INTR_IRQ0);
		return;
	} else if (nr == INTR_SWI0) {
		assert(iframe == kIFrame);
		hwsysintr();
		return;
	} else if (nr > INTR_SWI0) {
		assert(iframe == kIFrame);
#ifdef CONFIG_LOG_EXTRA_SWI
		printk("int %#x", nr);
		//dumpuser();
#endif
		return;
	} else switch (nr) {
	case ExceptionPageFault:
		panic("#PF from %#04x:%p at %#p (%d)",
				iframe[IFrame_CS], iframe[IFrame_EIP],
				getcr2(), iframe[IFrame_ErrorCode]);
	case ExceptionGeneralProtection:
		panic("#GP from %#04x:%p of instruction %02X",
				iframe[IFrame_CS], iframe[IFrame_EIP],
				*(volatile uchar*)iframe[IFrame_EIP]
				);
	case ExceptionInvalidOpcode:
		panic("#UD from %#04x:%p of instruction %02X",
				iframe[IFrame_CS], iframe[IFrame_EIP],
				*(volatile uchar*)iframe[IFrame_EIP]
				);
	case ExceptionBreakPoint:
		printk("Int3 from %#04x:%p (EAX=%p ESP=%p)",
				iframe[IFrame_CS], iframe[IFrame_EIP],
				iframe[IFrame_EAX], iframe[IFrame_ESP]
				);
		return;
	default:
		panic("Undefined Interrupt Number %d (%#x)", nr, nr);
	}

}
