#include <stdio.h>

int fputc(int c, FILE *fp)
{
	unsigned char ch = c;
	if (fwrite(&ch, sizeof(ch), 1, fp) != 1)
		return EOF;
	return ch;
}
