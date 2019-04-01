#include <stdio.h>

int fputc(int c, FILE *fp)
{
	char ch = c;
	if (fwrite(&ch, sizeof(ch), 1, fp) != 1)
		return EOF;
	if (c == '\n')
		fflush(fp);
	return ch;
}
