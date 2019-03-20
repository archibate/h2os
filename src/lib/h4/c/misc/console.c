#include <console.h>
#include <l4/types.h>
#include <l4/api/hello.h>

void con_putchar(char c)
{
	sys_con_putchar(c);
}

size_t con_write(const char *s, size_t len)
{
	sys_con_write(s, len);
	return len;
}
