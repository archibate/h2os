#include <stdio.h>

int fgetwc(FILE *fp)
{
	wchar_t ch;
	if (fread(&ch, sizeof(ch), 1, fp) != 1)
		return EOF;
	return ch;
}
