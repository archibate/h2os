#include <l4/system/kstack.h>
#include <l4/object/iframe.h>
#include <l4/misc/bug.h>
#include <l4/generic/task-switch.h>
#include <l4/generic/sched.h>

void _NORETURN move_to_user(void)
{
	task_switch(NULL, sched_get_curr());

	BUG_ON((kIFrame.cs & 3) != 3);
	asm volatile (
		"mov %0, %%esp\n"
		"jmp movusr_iframe_exiter\n"
		:: "r" (&kIFrame));
	UNREACHABLE();
}
