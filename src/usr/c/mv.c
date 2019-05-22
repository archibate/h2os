#include <stdio.h>
#include <unistd.h>
#include <errno.h>

void move(const char *src, const char *dst)
{
	int ret = link(src, dst);
	if (ret < 0) { errno = -ret; fprintf(stderr, "link %s->%s", src, dst); perror(""); return; }
	ret = unlink(src);
	if (ret < 0) { errno = -ret; fprintf(stderr, "unlink %s", src, dst); perror(""); unlink(dst); }
}

int main(int argc, char **argv)
{
	if (!argv[1] || !argv[2]) {
		fprintf(stderr, "mv: bad argument\n");
		return 1;
	}
	move(argv[1], argv[2]);
	return 0;
}