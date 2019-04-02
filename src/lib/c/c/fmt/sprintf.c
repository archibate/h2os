#include <gprintf.h>

static void mystrputc(char c, char **p)
{
	*(*p)++ = c;
}

int vsprintf
	( char *buf
	, const char *fmt
	, va_list ap
	)
{
	int ret = vgprintf(mystrputc, &buf, fmt, ap);
	mystrputc(0, &buf);
	return ret;
}


int sprintf
	( char *buf
	, const char *fmt
	, ...
	)
{
	va_list ap;
	va_start(ap, fmt);
	int res = vsprintf(buf, fmt, ap);
	va_end(ap);
	return res;
}
