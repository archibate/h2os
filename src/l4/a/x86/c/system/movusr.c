#include <l4/system/kstack.h>

void _NORETURN move_to_user(void)
{
	asm volatile (
		"mov %0, %%esp\n"
		"jmp iframe_exiter\n"
		:: "r" (&kIFrame));
	UNREACHABLE();
}
