#include <stdio.h>


int vprintf
	( const char *fmt
	, va_list ap
	)
{
	return vfprintf(stdout, fmt, ap);
}


int printf
	( const char *fmt
	, ...
	)
{
	va_list ap;
	va_start(ap, fmt);
	int res = vprintf(fmt, ap);
	va_end(ap);
	return res;
}
