#include <abort.h>
#include <l4/types.h>
#include <l4/api/hello.h>
#include <compiler.h>

void _NORETURN abort(void)
{
	sys_halt(); // exit(-1)
	UNREACHABLE();
}
