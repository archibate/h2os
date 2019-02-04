#include <drv/irqhw.h>
#include <drv/timer.h>
#include <k/x86/idt.h>
#include <x86/irq-nrs.h>

void init_kdrvs(void)
{
	irqhw_init(INTR_IRQ0);
	set_timer_freq(10);
	irq_enable(IRQ_TIMER);
}
