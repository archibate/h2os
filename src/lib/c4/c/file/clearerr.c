#include <stdio.h>

void clearerr(FILE *fp)
{
	fp->err = 0;
}
