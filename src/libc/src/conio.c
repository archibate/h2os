#include <conio.h>
#include <drv/tty.h>
#include <stdio.h>

int cputs(const char *s)
{
	tty_puts(s);
	return 0;
}

int cputchar(int ch)
{
	tty_putchar(ch);
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
