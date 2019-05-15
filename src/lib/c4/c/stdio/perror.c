#include <stdio.h>
#include <string.h>
#include <errno.h>

int perror(const char *s)
{
	fprintf(stderr, "%s: %s\n", s, strerror(errno));
	return 0;
}
