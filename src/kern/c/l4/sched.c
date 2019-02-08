#include <l4/sched.h>
#include <l4/a/tswitch.h>
#include <k/printk.h>
#include <k/panic.h>
#include <assert.h>

queue_t running;
tcb_t *currTcb;

void schedEnter(void)
{
	currTcb = schedGetCurr();
	assert(currTcb);
}

void schedLeave(void)
{
	tcb_t *next = schedGetCurr();
	if (next != currTcb)
		Arch_switchTask(currTcb, next);
}

void schedTimer(void)
{
	schedNext();
}
