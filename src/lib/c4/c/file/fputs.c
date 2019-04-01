#include <stdio.h>

int fputs(const char *s, FILE *fp)
{
	while (*s) {
		if (EOF == fputc(*s++, fp))
			return EOF;
	}
	return 1;
}
