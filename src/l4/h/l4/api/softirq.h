#pragma once

int sys_softirq_done(unsigned int irq);
int sys_softirq_set_enable(unsigned int irq, bool enable);
