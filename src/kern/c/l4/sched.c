#include <l4/sched.h>
#include <l4/a/tswitch.h>
#include <k/printk.h>
#include <k/panic.h>
#include <assert.h>

struct list_head *runningHead;
tcb_t *currTcb;

void schedInit(tcb_t *x)
{
	runningHead = &x->list;
	list_init(runningHead);
}

void schedSetActive(tcb_t *x)
{
	list_add(&x->list, runningHead);
}

void schedSetInactive(tcb_t *x)
{
	list_del(&x->list);
}

void schedEnter(void)
{
	currTcb = schedGetCurr();
	assert(currTcb);
}

void schedLeave(void)
{
	//dprintk("sl");
	tcb_t *next = schedGetCurr();
	if (next != currTcb)
		Arch_switchTask(currTcb, next);
}

void schedNext(void)
{
	//dprintk("sn");
	/*if (runningHead != runningHead->next)
		panic("!");*/
	runningHead = runningHead->next;
	/*if (schedGetCurr() != currTcb)
		panic("!");*/
}

void schedTimer(void)
{
	schedNext();
}
