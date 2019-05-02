#include <stdlib.h>
#include <string.h>
#include <stddef.h>

extern char *const *__crt_envp;

char *getenv(const char *name)
{
	for (p = __crt_envp; *p; p++) {
		char *end = strchrl(*p, '=');
		if (!strncmp(*p, name, end - *p))
			return *end ? end + 1 : end;
	}
	return NULL;
}
