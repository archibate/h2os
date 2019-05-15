#include <stdio.h>
#include <malloc.h>

void fdclose(FILE *fp)
{
	fflush(fp);
	hlist_del(&fp->hlist);
	free(fp);
	return;
}
