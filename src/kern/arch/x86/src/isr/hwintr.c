#include <k/x86/idt.h>
#include <k/asm/iframe.h>
#include <k/kstack.h>
#include <k/panic.h>
#include <assert.h>

void hwexcp(uint excp);
void hwirq(uint irq);
void hwsyscall(void);

void hwintr(ulong *iframe)
{
	uint nr = iframe[IFrame_IntrNum];

	if (nr >= INTR_IRQ0 && nr <= INTR_IRQMAX) {
		hwirq(nr - INTR_IRQ0);
		return;
	} else if (nr < INTR_IRQ0) {
		hwexcp(nr);
		return;
	} else if (nr == INTR_SWI0) {
		assert(iframe == kIFrame);
		hwsyscall();
		return;
	}

	panic("hwintr(%d/%#x)", nr, nr);
}

void hwexcp(uint nr)
{
	panic("hwexcp(%d/%#x)", nr, nr);
}

void hwirq(uint irq)
{
	printk("hwirq(%d/%#x)", irq, irq);
}
