#include <l4/generic/task-switch.h>
#include <l4/generic/pgdir.h>
//#include <l4/misc/printk.h>
//#include <l4/system/kstack.h>

void task_switch(struct ktcb *old_task, struct ktcb *new_task)
{
	pgdir_switch(new_task->pgdir, new_task->utcb);
	//printk("%p", kIFrame.pc);
	//printk("%p", kSEFrame.pc);
}
