#include <stdlib.h>
#include <c4/libcrt.h>

int main(int argc, char *const *argv, char *const *envp);

extern char __EBSS__[0];

void _NORETURN __crt_main(int argc, char *const *argv, char *const *envp)
{
	__crt_init(&__EBSS__, argc, argv, envp);
	exit(main(argc, argv, envp));
}
