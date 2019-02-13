#include <l4/machine/asm/irq-nrs.h>
#include <l4/generic/softirq.h>
#include <l4/generic/sched.h>
#include <l4/driver/irqhw.h>
#include <l4/misc/panic.h>

void hwirq(unsigned int irq)
{
	switch (irq) {
	case IRQ_TIMER:
		sched_timer_callback();
		irq_done(irq);
		break;
	}

	softirq_callback(irq);
}
