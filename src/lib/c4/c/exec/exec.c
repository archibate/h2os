#include <unistd.h>
#include <stddef.h>

int exec(const char *name)
{
	char *argv[] = {(char*)name, NULL};
	execv(name, argv);
}
