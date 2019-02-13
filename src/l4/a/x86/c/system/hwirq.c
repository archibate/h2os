#include <l4/machine/asm/irq-nrs.h>
#include <l4/generic/softirq.h>
#include <l4/generic/sched.h>
#include <l4/driver/irqhw.h>
#include <l4/misc/panic.h>

void hwirq(unsigned int irq)
{
	//BUG_ON(irq >= IRQ_MAX);
	switch (irq) {
	case IRQ_TIMER:
		sched_timer_callback();
		irq_done(irq);
		break;
	//default:
		//panic("Undefined IRQ %d (%#x)", irq, irq);
	}

	softirq_callback(irq);
}

#if 0 // {{{
void softirq_set_enable(unsigned int irq, bool enable)
{
	BUG_ON(irq >= IRQ_MAX);
	irq_set_enable(irq, enable);
}

void softirq_done(unsigned int irq)
{
	BUG_ON(irq >= IRQ_MAX);
	irq_done(irq);
}
#endif // }}}
