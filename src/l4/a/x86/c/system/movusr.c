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

void _NORETURN sched_halt(void)
{
	extern void __pgdir_switch_halt_utcb(void);
	__pgdir_switch_halt_utcb();
	asm volatile (
		"mov %0, %%esp\n"
		"sti;1:hlt;jmp 1b\n"
		:: "r" (&kIFrame.iframe_kern_top));
	UNREACHABLE();
}
