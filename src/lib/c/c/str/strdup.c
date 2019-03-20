#include <string.h>
#include <malloc.h>

char *strdup(const char *s)
{
	unsigned long n = strlen(s);
	char *d = malloc(n);
	strcpy(d, s);
	return d;
}

char *strndup(const char *s, unsigned long n)
{
	n = strnlen(s, n);
	char *d = malloc(n);
	strncpy(d, s, n);
	return d;
}
