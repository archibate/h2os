#include <l4/machine/asm/irq-nrs.h>
#include <l4/generic/sched.h>
#include <l4/misc/panic.h>

void hwirq(uint irq)
{
	switch (irq) {
	case IRQ_TIMER:
		sched_timer_callback();
		break;
	default:
		panic("Undefined IRQ %d (%#x)", irq, irq);
	}
}
