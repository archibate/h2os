#include <unistd.h>
#include <h4/mm.h>
#include <h4/fs.h>
#include <string.h>

extern char *const *__crt_envp;

int execvp(const char *path, char *const *argv)
{
	/*printk("path [%s]");
	char *const *p;
	for (p = argv; *p; p++)
		printk("arg#%d [%s]", p - argv, *p);*/
	execve(path, argv, __crt_envp);
}
