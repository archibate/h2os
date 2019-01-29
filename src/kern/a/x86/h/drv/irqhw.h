#pragma once

void irqhw_init(int irq0_intr);
void irq_setenable(int irq, int enable);
void irq_done(int irq);

#define irq_enable(irq)  irq_setenable(irq, 1)
#define irq_disable(irq) irq_setenable(irq, 0)
