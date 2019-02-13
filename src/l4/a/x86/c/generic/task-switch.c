#include <l4/generic/task-switch.h>
#include <l4/generic/pgdir.h>

void task_switch(struct ktcb *old_task, struct ktcb *new_task)
{
	pgdir_switch(new_task->pgdir, new_task->utcb);
}
