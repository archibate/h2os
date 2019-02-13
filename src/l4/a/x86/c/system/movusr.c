#include <l4/system/kstack.h>
#include <l4/object/iframe.h>
#include <l4/misc/bug.h>

void _NORETURN move_to_user(void)
{
	BUG_ON((kIFrame.cs & 3) != 3);
	asm volatile (
		"mov %0, %%esp\n"
		"jmp iframe_exiter\n"
		:: "r" (&kIFrame));
	UNREACHABLE();
}
