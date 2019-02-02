#include <libl4/puts.h>
#include <libl4/io.h>
#include <string.h>

int l4Puts(l4CPtr_t cptr, const char *s)
{
	return l4Write(cptr, s, strlen(s));
}
