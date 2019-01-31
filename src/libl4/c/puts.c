#include <libl4/puts.h>
#include <libl4/io.h>
#include <string.h>

int l4Puts(l4CPtr_t cptr, l4Word_t offset, const char *s)
{
	return l4Write(cptr, offset, s, strlen(s));
}
