#include <l4/sched.h>
#include <l4/a/tswitch.h>
#include <k/printk.h>
#include <k/panic.h>
#include <assert.h>

void _schedLowerPriority(void);

byte_t currPriority;
struct list_head *runningHeads[L4_MaxPriority];
tcb_t *currTcb;

void schedSetActive(tcb_t *x)
{
	if (!runningHeads[x->priority])
		list_init(runningHeads[x->priority] = &x->list);
	else
		list_add_tail(&x->list, runningHeads[x->priority]);
	if (x->priority > currPriority)
		currPriority = x->priority;
}

void schedSetInactive(tcb_t *x)
{
	if (&x->list == runningHead) {
		if (runningHead == runningHead->next) {
			runningHead = 0;
			_schedLowerPriority();
		} else {
			list_del_init(&x->list);
			schedNext();
		}
	} else {
		list_del(&x->list);
	}
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
	assert(next);
	//assert(next != (tcb_t*)HOLE_LIST);
	if (next != currTcb)
		Arch_switchTask(currTcb, next);
	//dprintk("sll");
	//dumpuser();
}

void _schedLowerPriority(void)
{
	int i;
	for (i = 0; i < currPriority; i++) {
		if (runningHeads[i]) {
			currPriority = i;
			dprintk("%d!", i);
			return;
		}
	}
	panic("_schedLowerPriority: Out of Task!");
}

void schedNext(void)
{
	runningHead = runningHead->next;
}

void schedTimer(void)
{
	dprintk("schedTimer...");
	schedNext();
}
