#include <stdio.h>

int fgetc(FILE *fp)
{
	unsigned char ch;
	if (fread(&ch, sizeof(ch), 1, fp) != 1)
		return EOF;
	return ch;
}
