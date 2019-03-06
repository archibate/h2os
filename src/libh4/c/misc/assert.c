#include <assert.h>
#include <conio.h>

#ifndef CONFIG_NOASSERT
void _NORETURN _assert_failed(const char *info, const char *file,
			      const char *func, int line)
{
	cprintf("%s:%d: %s: %s", file, line, func, info);
	UNREACHABLE();
}
#endif
