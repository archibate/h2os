#include <stdio.h>
#include <errno.h>

char *fgets(char *buf, size_t size, FILE *fp)
{
	int c = EOF;
	char *p = buf;
	while (p - buf < size - 1 && !feof(fp)) {
		*p++ = c = fgetc(fp);
		if (c == '\n')
			break;
	}
	*p++ = 0;
	return c == EOF ? NULL : buf;
}
