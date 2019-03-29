#include <stdio.h>

int ferror(FILE *fp)
{
	return fp->err;
}
