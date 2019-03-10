#include <h4/proc.h>
#include <l4/types.h>
#include <l4/api/sched.h>

int pause(void)
{
	sys_sched_next();
	return 0;
}
