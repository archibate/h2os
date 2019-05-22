#include <unistd.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void reader(FILE *fp)
{
	char buf[1024];
#if 1
	while (!feof(fp))
		putchar(fgetc(fp));
#else
	int ret = spawn("hex", NULL, NULL, NULL);
	if (ret < 0) { errno = -ret; perror("hex"); }
	//printk("spawned");
	//while (1) pause();
	wait();
	//printk("waited");
#endif
}

void writer(FILE *fp)
{
	FILE *fin = fopen("h4.txt", "r");
	char buf[1024];
	while (fgets(buf, sizeof(buf), fin))
		fputs(buf, fp);
	fclose(fin);
}

void server(void)
{
	int fd = mkpipe();
	char numfd[26];
	sprintf(numfd, "%d", fd);
	char *argv[] = {"piper", "--client-fd", numfd, NULL};
	int ret = spawn("piper", argv, NULL, NULL);
	if (ret < 0) { errno = -ret; perror("piper"); exit(ret); }
	pipctl(fd, 0);
	FILE *fp = fdopen(fd, "r");
	reader(fp);
	pipctl(fd, -1);
	fclose(fp);
	wait();
}

void client(int fd)
{
	pipctl(fd, 1);
	FILE *fp = fdopen(fd, "w");
	writer(fp);
	pipctl(fd, -1);
	fclose(fp);
}

int main(int argc, char **argv)
{
	if (argv[1] && !strcmp(argv[1], "--client-fd") && argv[2]) {
		client(atoi(argv[2]));
	} else {
		server();
	}
}
