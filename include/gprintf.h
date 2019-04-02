#pragma once


#include <stdarg.h>
#include <compiler.h>


int vgprintf
	( void (*putch)(char, void *)
	, void *arg
	, const char *fmt
	, va_list ap
	);

int gprintf
	( void (*putch)(char, void *)
	, void *arg
	, const char *fmt
	, ...
	) _FORMAT(printf, 3, 4);
