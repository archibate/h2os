#include <l4/generic/task-switch.h>
#include <l4/generic/pgdir.h>
//#include <l4/misc/printk.h>//
//#include <l4/system/kstack.h>//
//#include <l4/machine/mmu/mmu.h>//
//#include <l4/misc/bug.h>//

void task_switch(struct ktcb *old_task, struct ktcb *new_task)
{
	//printk("%d->%d", old_task->ide.id, new_task->ide.id);
	//BUG_ON((word_t) old_task->pgdir != mmu_getPgdirPaddr());//
	//printk("%p->%p", old_task->pgdir, new_task->pgdir);//
	pgdir_switch(new_task->pgdir, new_task->utcb);
	//printk("%p", kIFrame.pc);//
	//printk("%p", kSEFrame.pc);//
}
