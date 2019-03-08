#include <assert.h>
#include <printk.h>

#ifndef CONFIG_NOASSERT
void _NORETURN _assert_failed(const char *info, const char *file,
			      const char *func, int line)
{
	printk("%s:%d: %s: %s", file, line, func, info);
	UNREACHABLE();
}
#endif
