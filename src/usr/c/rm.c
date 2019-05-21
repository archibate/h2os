#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void rm(const char *path)
{
	int ret = unlink(path);
	if (ret < 0) { errno = -ret; perror(path); }
}

int main(int argc, char **argv)
{
	if (!*++argv) fprintf(stderr, "rm: missing arguments\n");
	else while (*argv) rm(*argv++);
	return 0;
}
