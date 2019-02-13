#include <l4/api/softirq.h>
#include <l4/enum/irq-nrs.h>
#include <l4/driver/irqhw.h>
#include <l4/api/softirq.h>
#include <l4/enum/errno.h>

int sys_softirq_done(unsigned int irq)
{
	if (irq >= IRQ_MAX)
		return -EINVAL;

	if (irq == IRQ_TIMER)
		return -EPERM;

	irq_done(irq);
	return 0;
}

int sys_softirq_set_enable(unsigned int irq, bool enable)
{
	if (irq >= IRQ_MAX)
		return -EINVAL;

	if (irq == IRQ_TIMER)
		return -EPERM;

	irq_setenable(irq, enable);
	return 0;
}
