#include <l4/driver/irqhw.h>
#include <l4/driver/timer.h>
#include <l4/system/asm/idt.h>
#include <l4/machine/asm/irq-nrs.h>

void init_kdrvs(void)
{
	irqhw_init(INTR_IRQ0);
	set_timer_freq(10);
	irq_enable(IRQ_TIMER);
}
