#include <h4/sys/types.h>
#include <h4/sys/sched.h>
#include <l4/types.h>
#include <l4/api/sched.h>

pid_t getpid(void)
{
	return sys_getpid();
}
