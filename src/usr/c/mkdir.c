#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void do_mkdir(const char *path)
{
	int ret = mkdir(path, 0);
	if (ret < 0) { errno = -ret; perror(path); }
}

int main(int argc, char **argv)
{
	if (!*++argv) fprintf(stderr, "touch: missing arguments\n");
	else while (*argv) do_mkdir(*argv++);
	return 0;
}
