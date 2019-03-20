#include <h4/proc.h>
#include <l4/types.h>
#include <l4/api/sched.h>

pid_t getpid(void)
{
	return sys_getpid();
}
