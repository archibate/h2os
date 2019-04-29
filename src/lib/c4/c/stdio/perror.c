#include <stdio.h>
#include <string.h>
#include <errno.h>

int perror(const char *s)
{
	return fprintf(stderr, "%s: %s", s, strerror(errno));
}
