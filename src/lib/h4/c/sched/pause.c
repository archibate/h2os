#include <h4/sys/types.h>
#include <h4/sys/sched.h>
#include <l4/types.h>
#include <l4/api/sched.h>

int pause(void)
{
	sys_pause();
	return 0;
}
