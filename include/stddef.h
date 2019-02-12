#pragma once

#ifndef NULL
#define NULL ((void *)0)
#endif
#ifndef EOF
#define EOF (-1)
#endif
#define offsetof(type, member) ((unsigned long) &(((type*)0)->member))
#define container_of(ptr, type, member) ({ \
		typeof(((type*)0)->member) *_$p = (ptr); \
		(type*)((char*)_$p - offsetof(type, member)); \
	})
#define ARRAY_SIZEOF(a) (sizeof(a) / sizeof((a)[0]))
#define array_sizeof(a) (sizeof(a) / sizeof((a)[0]))
