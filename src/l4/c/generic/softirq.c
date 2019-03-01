#include <l4/generic/softirq.h>
#include <l4/generic/asyncep.h>
#include <l4/generic/idget.h>
#include <l4/generic/sched.h>
#include <l4/enum/irq-nrs.h>
#include <l4/enum/rtype.h>
#include <l4/misc/bug.h>
#include <l4/misc/panic.h>

struct async_ep softirq_aeps[IRQ_MAX];

void init_softirq_aeps(void)
{
	unsigned int i;
	for (i = 0; i < IRQ_MAX; i++) {
		struct async_ep *aep = &softirq_aeps[i];
		async_init(aep);
		aep->ide.id = i;
		BUG_ON(!idg_set_entry(&aep->ide, RTYPE_ASYNC_EP));
	}
}

void softirq_callback(unsigned int irq)
{
	sched_enter();
	BUG_ON(irq >= IRQ_MAX);
	async_pulse(&softirq_aeps[irq]);
	sched_leave();
}
