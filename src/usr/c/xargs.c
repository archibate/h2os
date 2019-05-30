#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <spawn.h>
#include <errno.h>

char *replace(const char *src, const char *s)
{
	int n = 0, len = strlen(s);
	char *res, *r;
	const char *p = src;
	while ((p = strchr(p, '%')))
		p++, n++;
	r = res = malloc(strlen(src) + (len - 1) * n + 1);
	while ((p = strchr(src, '%'))) {
		memcpy(r, src, p - src);
		r += p - src;
		memcpy(r, s, len);
		r += len;
		src = p + 1;
	}
	strcpy(r, src);
	return res;
}

void process(char **args, char *const *argv, const char *str)
{
	while (*argv)
		*args++ = replace(*argv++, str);
	*args = NULL;
}

void destroy(char **args)
{
	while (*args)
		free(*args++);
}

int main(int argc, char **argv)
{
	char buf[1024];
	argv++, argc--;
	char *args[argc];
	while (fgets(buf, sizeof(buf), stdin)) {
		if (buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = 0;
		process(args, argv, buf);
		int ret = spawn(args[0], args, NULL, NULL);
		if (ret < 0) { errno = -ret; perror(args[0]); }
		else wait();
		destroy(args);
	}
	if (ferror(stdin)) perror("-");
}
