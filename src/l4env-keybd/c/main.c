#include <l4/types.h>
#include <l4/defines.h>
#include <l4/api/hello.h>
#include <l4/api/sched.h>
#include <l4/api/asyncep.h>
#include <l4/api/softirq.h>
#include <l4/enum/irq-nrs.h>

void main(void)
{
	sys_print("KEYBD!!!");

	sys_softirq_set_enable(IRQ_KEYBD, true);
	sys_async_listen(IRQ_KEYBD);
	sys_softirq_done(IRQ_KEYBD);

	for (;;)
		sys_putchar('K');

	sys_exit();
}
