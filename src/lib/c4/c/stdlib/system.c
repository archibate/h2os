#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <spawn.h>

int system(const char *cmd)
{
	char *argv[] = {"sh", "-c", (char*)cmd, NULL};
	pid_t pid = spawn(argv[0], argv, NULL, NULL);
	if (pid < 0)
		return pid;
	wait();
	return 0;
}
