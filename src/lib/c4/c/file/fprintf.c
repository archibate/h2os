#include <gprintf.h>
#include <stdio.h>


int vfprintf
	( FILE *fp
	, const char *fmt
	, va_list ap
	)
{
	return vgprintf(fputc, fp, fmt, ap);
}


int fprintf
	( FILE *fp
	, const char *fmt
	, ...
	)
{
	va_list ap;
	va_start(ap, fmt);
	int res = vfprintf(fp, fmt, ap);
	va_end(ap);
	return res;
}
