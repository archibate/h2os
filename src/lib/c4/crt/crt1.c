#include <c4/libcrt.h>

int main(int argc, char *const *argv, char *const *envp);

extern char __EBSS__[0];

void _NORETURN __crt_main(void *sp)
{
	__crt_start(&__EBSS__, main, sp);
}
