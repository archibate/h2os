#include <l4/api/hello.h>
#include <l4/misc/printk.h>
#include <l4/machine/asm/clsti.h>
#include <l4/driver/console.h>

int sys_hello(void)
{
	printk("HELLO, WORLD!");
	return 233;
}

int sys_putchar(char c)
{
	//con_setcur(0, 0);
	con_putchar(c);
	return 233;
}

int sys_print(const char *s)
{
	printk("sys_print: %s", s);
	return 233;
}

int sys_halt(void)
{
	printk("sys_halt() called");
	clihlt();
	UNREACHABLE();
}
