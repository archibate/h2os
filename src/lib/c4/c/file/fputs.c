#include <stdio.h>

int fputs(const char *s, FILE *fp)
{
	return fwrite(s, strlen(s), 1, fp) ? 1 : EOF;
}
