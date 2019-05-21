#include <stdio.h>
#include <unistd.h>
#include <errno.h>

void do_symlink(const char *src, const char *dst)
{
	int ret = symlink(src, dst);
	if (ret < 0) { errno = -ret; fprintf(stderr, "lns %s %s", src, dst); perror(""); }
}

int main(int argc, char **argv)
{
	if (!argv[1] || !argv[2]) {
		fprintf(stderr, "lns: bad argument\n");
		return 1;
	}
	do_symlink(argv[1], argv[2]);
	return 0;
}
