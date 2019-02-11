// https://blog.csdn.net/forestcell/article/details/15814591
#include <drv/console.h>
#include <inttypes.h>
#include <arm/io.h>
#include <conio.h>

#define UART 0x1c090000

void con_init(void)
{
	outl(UART+0x24, 0x10);
	outl(UART+0x30, 0xc301);
}

size_t con_write(const char *s, size_t n)
{
	while (n--)
		con_putchar(*s++);
}

void con_putchar(char c)
{
	outl(UART, c);
}

void con_clear(void)
{
	con_write(ANSI_CLEAR);
}

void con_setcur(int x, int y)
{
	cprintf(ANSI_SETCUR_YX, y, x);
}

//void con_getcur(int *px, int *py);
