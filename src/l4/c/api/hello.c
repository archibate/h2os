#include <l4/api/hello.h>
#include <l4/misc/printk.h>

int sys_hello(void)
{
	printk("HELLO, WORLD!");
	return 233;
}
