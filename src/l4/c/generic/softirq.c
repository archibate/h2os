#include <l4/generic/softirq.h>
#include <l4/generic/asyncep.h>
#include <l4/generic/idget.h>
#include <l4/enum/irq-nrs.h>
#include <l4/enum/rtype.h>
#include <l4/misc/bug.h>

struct async_ep softirq_aeps[IRQ_MAX];

void init_softirq_aeps(void)
{
	unsigned int i;
	for (i = 0; i < IRQ_MAX; i++) {
		struct async_ep *aep = &softirq_aeps[i];
		async_init(aep);
		aep->ide.id = i;
		bool r = idg_set_entry(&aep->ide, RTYPE_ASYNC_EP);
		BUG_ON(!r);
	}
}

void softirq_callback(unsigned int irq)
{
	BUG_ON(irq >= IRQ_MAX);
	async_pulse(&softirq_aeps[irq]);
}
