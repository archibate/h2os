#include <stdio.h>

off_t ftell(FILE *fp)
{
	return fseek(fp, 0, SEEK_CUR);
}
