#include <h4/proc/api.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/filedes.h>

void *sbrk(ptrdiff_t incptr)
{
	ipc_rewindw(_PROC_sbrk);
	ipc_putw(incptr);
	ipc_call(SVFD_PROC);
	return (void*)ipc_getw();
}