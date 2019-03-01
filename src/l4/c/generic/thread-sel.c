#include <l4/generic/sched.h>
#include <l4/generic/task-switch.h>
#include <l4/machine/asm/clsti.h>
#include <l4/misc/bug.h>

void sched_enter(void)
{
	BUG_ON(running_head == NULL);
	BUG_ON(running_head == INVALID_PTR);
	current = sched_get_curr();
}

void sched_leave(void)
{
	while (curr_idle)
		sched_halt();

	BUG_ON(running_head == NULL);
	BUG_ON(running_head == INVALID_PTR);
	struct ktcb *next = sched_get_curr();
	if (next != current) {
		//printk("slts %p->%p", current, next);
		task_switch(current, next);
	}
	current = NULL;
}
