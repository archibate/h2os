#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <console.h>

int cputs(const char *s)
{
	con_write(s, strlen(s));
	return 0;
}

int cputchar(int ch)
{
	con_putchar(ch);
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
