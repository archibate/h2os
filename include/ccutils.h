#pragma once

#define _DEPRECATED		__attribute__((deprecated))
#define _FASTCALL	    __attribute__((fastcall))
#define _STDCALL	    __attribute__((stdcall))
#define _CDECL  	    __attribute__((cdecl))
#define _PURE			    __attribute__((const))
#define _WEAK		      __attribute__((weak))
#define _PACKED			  __attribute__((packed))
#define _UNUSED			  __attribute__((unused))
#define _NORETURN     __attribute__((noreturn))
#define _INLINE 		  __attribute__((always_inline))
#define _FORMAT(...)	__attribute__((format(__VA_ARGS__)))
#define _SECTION(x) 	__attribute__((section(x)))
#define _ALIGNED(x) 	__attribute__((aligned(x)))

#if !defined(_MINGW)
#define UNREACHABLE()		__builtin_unreachable()
#else
#define UNREACHABLE()		for (;;)
#endif

#if !defined(_MINGW)
#define likely(x)			(__builtin_expect(!!(x), 1))
#define unlikely(x)			(__builtin_expect(!!(x), 0))
#else
#define likely(x)			(!!(x))
#define unlikely(x)			(!!(x))
#endif
