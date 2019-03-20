#include <l4/generic/task-switch.h>
#include <l4/generic/pgdir.h>
#include <l4/generic/context.h>
#include <l4/system/kstack.h>

void task_switch(struct ktcb *old_task, struct ktcb *new_task)
{
	pgdir_switch(new_task->pgdir, new_task->ipcbuf);

	kUTCB->msginfo = new_task->msginfo;

	if (old_task != new_task) {
		if (old_task != NULL)
			save_context(&old_task->context);
		restore_context(&new_task->context);
	}
}

void set_idle_task(struct ktcb *old_task)
{
	if (old_task != NULL)
		save_context(&old_task->context);

	extern void _NORETURN idle_exiter(void);
	kErnelExiter = idle_exiter;
}
