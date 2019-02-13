#include <l4/types.h>
#include <l4/api/hello.h>
#include <l4/api/sched.h>

void main(void)
{
	sys_print("KEYBD!!!");
	sys_hello();

	for (;;)
		sys_putchar('k');

	sys_exit();
}
