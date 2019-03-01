#include <l4/generic/sched.h>
#include <l4/generic/task-switch.h>
#include <l4/machine/asm/clsti.h>
#include <l4/misc/bug.h>

void sched_enter(void)
{
	if (curr_idle) {
		current = NULL;
		return;
	}

	BUG_ON(running_head == NULL);
	BUG_ON(running_head == INVALID_PTR);

	current = sched_get_curr();
}

void sched_leave(void)
{
	if (curr_idle) {
		BUG_ON(sched_get_curr() != NULL);
		current = NULL;
		sched_halt();
		UNREACHABLE();
	}

	BUG_ON(running_head == NULL);
	BUG_ON(running_head == INVALID_PTR);
	struct ktcb *next = sched_get_curr();

	if (next != current) {
		task_switch(current, next);
	}

	current = NULL;
}
