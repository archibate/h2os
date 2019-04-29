#include <h4/fs.h>
#include <h4/file.h>
//#include <h4/sys/execve.h>
#include <stdio.h>

int main(int argc, char *const argv[], char *const envp[])
{
	close(0);
	close(1);
	close(2);
	open("/dev/keybd", O_RDONLY);
	open("/dev/cons", O_WRONLY);
	dup(1);

	printf("init: Hello, World!\n");
	return 0;
	/*char *exec_argv[] = {"hello", NULL};
	return execve("/bin/hello", &exec_argv, envp);*/
}
