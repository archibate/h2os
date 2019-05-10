#include <c4/libcrt.h>
#include <compiler.h>

int main(int argc, char *const *argv, char *const *envp);

extern char __EBSS__[0];

#ifdef _MINGW
#define _start start
#endif
void _NORETURN _start(void *a)
{
	__crt_start(&__EBSS__, main, &a - 1);
	UNREACHABLE();
}
