#include <l4/machine/asm/irq-nrs.h>
#include <l4/misc/panic.h>

void hwirq(uint irq)
{
	switch (irq) {
	case IRQ_TIMER:
		//systemTimer();
		//break;
	default:
		printk("Undefined IRQ %d (%#x)", irq, irq);
	}
}
