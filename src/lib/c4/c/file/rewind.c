#include <stdio.h>
#include <bug.h>

int rewind(FILE *fp)
{
	return fseek(fp, 0, SEEK_SET);
}
