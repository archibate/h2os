#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
	if (*++argv) exit(execv(*argv, argv));
	while (*envp) puts(*envp++);
}
