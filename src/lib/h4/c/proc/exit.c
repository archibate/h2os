#include <h4/proc.h>
#include <l4/types.h>
#include <l4/api/sched.h>
#include <compiler.h>

void _NORETURN _exit(int status)
{
	sys_exit();
	UNREACHABLE();
}
