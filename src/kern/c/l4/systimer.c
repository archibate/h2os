#include <l4/systimer.h>
#include <l4/sched.h>

void systemTimer(void)
{
	schedEnter();
	schedTimer();
	schedLeave();
}
