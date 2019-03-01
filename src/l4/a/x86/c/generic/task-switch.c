#include <l4/generic/task-switch.h>
#include <l4/generic/pgdir.h>

extern bool ep_transfered;

void task_switch(struct ktcb *old_task, struct ktcb *new_task)
{
	pgdir_switch(new_task->pgdir,
			new_task->utcb,
			new_task->ipcbuf);
}

#if 0
#include <l4/generic/sched.h>
#include <l4/system/kstack.h>

void user_ipc_enter(void)
{
	void *buf = curr->ipc.buf;
	size_t len = curr->ipc.len;

	if (buf == NULL || !len)
		return;

	memcpy(buf, kIPCBuffer, len);
}
#endif
