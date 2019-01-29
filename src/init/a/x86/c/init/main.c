#include <libl4/service.h>

void init_main(void)
{
	static char s[] = "Hello, seL4!\n";
	l4Send(0, 0, s, sizeof(s));
	for (;;);
}
