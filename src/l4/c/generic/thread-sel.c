#include <l4/generic/sched.h>
#include <l4/generic/task-switch.h>
#include <l4/machine/asm/clsti.h>
#include <l4/misc/bug.h>
#include <l4/misc/printk.h>

static int setimes;
static void *old_ipcbuf;

#undef sched_enter
void sched_enter(void)
{
	BUG_ON(setimes++);

	if (curr_idle) {
		current = NULL;
		return;
	}

	BUG_ON(running_head == NULL);
	BUG_ON(running_head == INVALID_PTR);

	current = sched_get_curr();

	old_ipcbuf = current->ipcbuf;
}

void sched_leave(void)
{
	BUG_ON(--setimes);

	if (curr_idle) {
		BUG_ON(sched_get_curr() != NULL);
		set_idle_task(current);
		//if (current) printk("task_switch %p->%p", current, NULL);
		current = NULL;
		return;
	}

	BUG_ON(running_head == NULL);
	BUG_ON(running_head == INVALID_PTR);
	struct ktcb *next = sched_get_curr();

	if (next != current || old_ipcbuf != current->ipcbuf) {
		//printk("task_switch %p->%p", current, next);
		task_switch(current, next);
	}

	current = NULL;
}
