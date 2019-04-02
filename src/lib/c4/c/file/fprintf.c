#include <gprintf.h>
#include <stdio.h>


int vfprintf
	( FILE *fp
	, const char *fmt
	, va_list ap
	)
{
	int ret = vgprintf(fputc, fp, fmt, ap);

	mystrputc(0);

	return ret;
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
