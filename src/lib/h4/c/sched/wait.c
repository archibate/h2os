#include <h4/sys/types.h>
#include <h4/sys/sched.h>
#include <l4/types.h>
#include <l4/api/sched.h>

int wait(void)
{
	while (sys_wait_first() > 1);
	return 0;
}
