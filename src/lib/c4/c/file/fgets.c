#include <stdio.h>
#include <errno.h>

char *fgets(char *buf, size_t size, FILE *fp)
{
	int c;
	char *p = buf;
	while (p - buf < size - 1) {
		c = fgetc(fp);
		if (feof(fp)) {
			buf = NULL;
			break;
		}
		*p++ = c;
		if (c == '\n')
			break;
	}
	*p++ = 0;
	return buf;
}
