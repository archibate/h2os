#include <unistd.h>
#include <spawn.h>
#include <stddef.h>
#include <fcntl.h>

int main(int argc, char *const *argv, char *const *envp)
{
	close(0);
	close(1);
	close(2);
	open("/dev/keybd", O_RDONLY);
	open("/dev/cons", O_WRONLY);
	dup(1);

	char *sh_argv[] = {"-sh", NULL};
	spawnp("/sh", sh_argv, envp, NULL);
	wait();
	return 1;
}
