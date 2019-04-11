#include <h4/sys/types.h>
#include <h4/sys/sched.h>
#include <l4/types.h>
#include <l4/api/sched.h>
#include <compiler.h>

void _NORETURN _exit(int status)
{
	sys_exit();
	UNREACHABLE();
}
