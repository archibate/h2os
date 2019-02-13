#include <l4/api/hello.h>
#include <l4/misc/printk.h>
#include <l4/machine/asm/clsti.h>

int sys_hello(void)
{
	printk("HELLO, WORLD!");
	return 233;
}

int sys_print(const char *s)
{
	printk("sys_print: %s");
	return 233;
}

int sys_halt(void)
{
	printk("sys_halt() called");
	clihlt();
	UNREACHABLE();
}
