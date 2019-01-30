#include <libl4/invoke.h>
#include <l4/services.h>
#include <l4/inicaps.h>

void init_main(void)
{
	static char s[] = "Hello, seL4!\n";
	l4Send(L4_InitCapConsole, L4_Write, 0, s, sizeof(s) - 1);
	l4Send(L4_InitCapDebug, L4_Debug_Puts, 0, s, sizeof(s) - 1);
	l4Send(L4_InitCapDebug, L4_Debug_Halt, 0, 0, 0);
}
