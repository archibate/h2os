#include <stdio.h>
#include <malloc.h>

void fdclose(FILE *fp)
{
	free(fp);
	return;
}
