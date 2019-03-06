#pragma once

#ifdef CONFIG_NOASSERT

#define _assert_failed(...) UNREACHABLE()

#else

#include <compiler.h>
void _NORETURN _assert_failed(const char *info, const char *file,
			      const char *func, int line);

#endif

#define assert_info(x, info) \
	do { if (!(x)) \
		_assert_failed(info, __FILE__, __FUNCTION__, __LINE__); \
	} while (0)

#define assert(x)  assert_info(x, "assert("#x") failed")
