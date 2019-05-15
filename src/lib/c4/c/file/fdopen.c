#include <stdio.h>
#include <compiler.h>
#include <malloc.h>
#include <string.h>

static struct hlist_head files;

FILE *fdopen(int fd, const char *mode)
{
	FILE *fp = malloc(sizeof(FILE));
	hlist_add_head(&fp->hlist, &files);
	fp->fd = fd;
	fp->err = 0;
	fp->eof = 0;
	fp->wr = NULL != strchr(mode, 'w');
	return fp;
}

static void _DTOR files_dtor(void)
{
	FILE *fp;
	hlist_for_each_entry2(fp, &files, hlist) {
		fdclose(fp);
	}
}
