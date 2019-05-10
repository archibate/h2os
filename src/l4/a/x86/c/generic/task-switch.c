#include <l4/generic/task-switch.h>
#include <l4/generic/pgdir.h>
#include <l4/generic/context.h>
#include <l4/generic/msginfo.h>
#include <l4/system/kstack.h>
#include <l4/misc/bug.h>

void task_switch(struct ktcb *old_task, struct ktcb *new_task)
{
#if 0
	if (new_task->bg_forked) {
		printk("pre-bg_forked:");
		printk("(pc=%p)", old_task->context.pc);
		printk("(sp=%p)", old_task->context.sp);
		printk("(sp[1]=%p)", ((void**)old_task->context.sp)[1]);
	}
#endif
	pgdir_switch(new_task->mm->pgdir, new_task->ipcbuf);
#if 0
	if (new_task->bg_forked) {
		printk("newly-bg_forked!!");
		printk("(pc=%p)", new_task->context.pc);
		printk("(sp=%p)", new_task->context.sp);
		printk("(sp[1]=%p)", ((void**)new_task->context.sp)[1]);
		BUG();
		new_task->bg_forked = false;
	}
#endif

	msginfo_copy(&kUTCB->msginfo, &new_task->msginfo);

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
