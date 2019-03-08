#include <l4/api/hello.h>
#include <l4/misc/printk.h>
#include <l4/machine/asm/clsti.h>
#include <l4/driver/console.h>

int sys_hello(void)
{
	printk("HELLO, WORLD!");
	return 233;
}

int sys_print(const char *s)
{
	printk("sys_print: %s", s);
	return 233;
}

int sys_con_putchar(char c)
{
	con_putchar(c);
	return 233;
}

int sys_con_write(const char *s, size_t len)
{
	con_write(s, len);
	return 233;
}

int sys_halt(void)
{
	printk("sys_halt() called");
	clihlt();
	UNREACHABLE();
}
