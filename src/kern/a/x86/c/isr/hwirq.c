#include <x86/irq-nrs.h>
#include <l4/systimer.h>
#include <k/printk.h>
#include <k/panic.h>
#include <assert.h>

void hwirq(uint irq)
{
	switch (irq) {
	case IRQ_TIMER:
		systemTimer();
		break;
	default:
		printk("Undefined IRQ %d (%#x)", irq, irq);
	}
}
