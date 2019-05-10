#include <stdio.h>
#include <malloc.h>

void fdclose(FILE *fp)
{
	hlist_del(&fp->hlist);
	free(fp);
	return;
}
