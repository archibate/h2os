#include <h4/sys/types.h>
#include <h4/sys/sched.h>
#include <l4/types.h>
#include <l4/api/sched.h>
#include <l4/api/ipc.h>

int wait(void)
{
	while (sys_wait_first() > 1)
		sys_reply(0, 0, 0);
	return 0;
}
