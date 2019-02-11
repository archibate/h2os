#include <l4/misc/assert.h>
#include <l4/misc/panic.h>

#ifndef CONFIG_NOASSERT
void _NORETURN _assert_failed(const char *info, const char *file,
			      const char *func, int line)
{
	panic("%s:%d: %s: %s", file, line, func, info);
}
#endif
