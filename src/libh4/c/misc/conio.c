#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <l4/api/hello.h>

int cputs(const char *s)
{
	sys_print(s);
	return 0;
}

int cputchar(int ch)
{
	char s[2];
	s[0] = ch;
	s[1] = 0;
	sys_print(s);
	return 0;
}

int vcprintf(const char *fmt, va_list ap)
{
	char buf[256];
	int ret = vsprintf(buf, fmt, ap);
	cputs(buf);
	return ret;
}

int cprintf(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int ret = vcprintf(fmt, ap);
	va_end(ap);
	return ret;
}
