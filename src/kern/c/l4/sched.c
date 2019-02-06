#include <l4/sched.h>
#include <l4/a/tswitch.h>
#include <k/printk.h>
#include <k/panic.h>

queue_t running;
tcb_t *currTcb;

void schedLeave(void)
{
	tcb_t *next = schedGetCurr();
	if (next != currTcb)
		switch_task(currTcb, next);
}
