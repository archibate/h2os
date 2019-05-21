#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void touch(const char *path)
{
	int fd = open(path, O_CREAT);
	if (fd < 0) { errno = -fd; perror(path); }
	else close(fd);
}

int main(int argc, char **argv)
{
	if (!*++argv) fprintf(stderr, "touch: missing arguments\n");
	else while (*argv) touch(*argv++);
	return 0;
}
