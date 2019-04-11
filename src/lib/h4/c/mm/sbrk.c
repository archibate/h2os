#include <h4/mm.h>
#include <h4/mm/sysnr.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/filedes.h>

void *sbrk(ptrdiff_t incptr)
{
	ipc_rewindw(_MM_sbrk);
	ipc_putw(incptr);
	ipc_call(SVFD_MM);
	return (void*)ipc_getw();
}
