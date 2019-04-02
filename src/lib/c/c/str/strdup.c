#include <string.h>
#include <malloc.h>

char *strdup(const char *s)
{
	unsigned long n = strlen(s) + 1;
	char *d = malloc(n * sizeof(char));
	strcpy(d, s);
	return d;
}

char *strndup(const char *s, unsigned long n)
{
	n = strnlen(s, n) + 1;
	char *d = malloc(n * sizeof(char));
	strncpy(d, s, n);
	return d;
}
