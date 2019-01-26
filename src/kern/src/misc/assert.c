#include <assert.h>
#include <k/panic.h>

void _NORETURN _assert_failed(const char *info, const char *file,
							 const char *func, int line)
{
	panic("%s:%d: %s: %s", file, line, func, info);
}
