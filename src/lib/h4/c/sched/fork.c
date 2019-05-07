#include <h4/sys/types.h>
#include <h4/sys/sched.h>
#include <l4/types.h>
#include <l4/api/sched.h>

pid_t fork(void)
{
	return sys_fork();
}
