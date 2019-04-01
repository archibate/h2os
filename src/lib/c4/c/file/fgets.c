#include <stdio.h>

char *fgets(char *buf, size_t size, FILE *fp)
{
	int c;
	char *p = buf;
	while (p - buf < size - 1) {
		c = fgetc(fp);
		if (c == EOF) {
			buf = NULL;
			break;
		}
		*p++ = (unsigned char) c;
		if (c == '\n')
			break;
	}
	*p++ = 0;
	return buf;
}
