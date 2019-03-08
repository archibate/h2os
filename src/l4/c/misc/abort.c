#include <abort.h>
#include <l4/machine/asm/clsti.h>

void _NORETURN abort(void)
{
	clihlt();
}
