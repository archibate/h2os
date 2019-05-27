#include <unistd.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void writer(FILE *fp)
{
	FILE *fin = fopen("h4.txt", "r");
	char buf[1024];
	while (fgets(buf, sizeof(buf), fin))
		fputs(buf, fp);
	fclose(fin);
}

int main(int argc, char **argv)
{
	argv++, argc--;
	if (!argv[0]) { fprintf(stderr, "piper: missing argument\n"); }
	int fd[2];
	pipe(fd);
	close(0);
	dup(fd[0]);
	close(fd[0]);
	int ret = spawn(argv[0], argv, NULL, NULL);
	if (ret < 0) { errno = -ret; perror(argv[0]); exit(ret); }
	close(0);
	FILE *fp = fdopen(fd[1], "w");
	writer(fp);
	pipctl(fd[1], -1);
	fclose(fp);
	wait();
}
