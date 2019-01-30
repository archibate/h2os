#pragma once

#ifdef CONFIG_NOASSERT
#define assert_info(x, info) /* Nothing */
#else
#include <ccutils.h>
void _NORETURN _assert_failed(const char *info, const char *file, const char *func, int line);
#define assert_info(x, info) (unlikely(x) ? (void)0 : \
	_assert_failed(info, __FILE__, __FUNCTION__, __LINE__))
#endif

#define assert(x) assert_info(x, "assert("#x") failed")
