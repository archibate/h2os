#include <assert.h>
#include <printk.h>
#include <abort.h>
#include <backtrace.h>

#ifndef CONFIG_NOASSERT
void _NORETURN _assert_failed(const char *info, const char *file,
			      const char *func, int line)
{
	printk("%s:%d: %s: %s", file, line, func, info);
	backtrace();
	abort();
	UNREACHABLE();
}
#endif
