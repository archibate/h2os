#include <stdlib.h>

int main(int argc, char *const argv[], char *const envp[]);
void __crt_init(int argc, char *const argv[], char *const envp[]);

void _NORETURN __crt_main(int argc, char *const argv[], char *const envp[])
{
	__crt_init(argc, argv, envp);
	exit(main(argc, argv, envp));
}
