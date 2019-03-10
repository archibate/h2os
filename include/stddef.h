#pragma once

#ifndef NULL
#define NULL         ((void *)0)
#endif
#ifndef INVALID_PTR
#define INVALID_PTR  ((void*)-1)
#endif
#ifndef true
#define true         ((bool)1)
#endif
#ifndef false
#define false        ((bool)0)
#endif
#ifndef EOF
#define EOF          (-1)
#endif
#ifndef WEOF
#define WEOF         ((wchar_t)-1)
#endif
#define offsetof(type, member) ((unsigned long) &(((type*)0)->member))
#define container_of(ptr, type, member) ({ \
		typeof(((type*)0)->member) *_$p = (ptr); \
		(type*)((char*)_$p - offsetof(type, member)); \
	})
#define ARRAY_SIZEOF(a) (sizeof(a) / sizeof((a)[0]))
#define array_sizeof(a) (sizeof(a) / sizeof((a)[0]))
