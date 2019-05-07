#include <h4/fs.h>
#include <h4/file.h>
#include <h4/mm.h>
#include <h4/sys/types.h>
#include <h4/sys/sched.h>
#include <stdio.h>

int main(int argc, char *const *argv, char *const *envp)
{
	close(0);
	close(1);
	close(2);
	open("/dev/keybd", O_RDONLY);
	open("/dev/cons", O_WRONLY);
	dup(1);

	printf("init started\n");

	char *exec_argv[] = {"echo", "hello", "world", NULL};
	char *exec_envp[] = {"PATH=/", NULL};
	if (!fork())
		execve("/echo", exec_argv, exec_envp);
	else
		wait();
	printf("error: init process exiting\n");
	return 1;
}
