#include <stdlib.h>
#include <h4/sys/types.h>
#include <h4/sys/sched.h>
#include <compiler.h>

static void call_dtors(void)
{
	typedef void (*dtor_ptr_t)(void);
	extern dtor_ptr_t __DTORS_BEGIN__[], __DTORS_END__[];
	dtor_ptr_t *dtor;
	for (dtor = __DTORS_BEGIN__; dtor < __DTORS_END__; dtor++)
		(*dtor)();
}

void _NORETURN exit(int status)
{
	call_dtors();
	_exit(status);
}
