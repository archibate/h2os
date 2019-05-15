#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>

int main(int argc, char **argv, char **envp)
{
	if (*++argv) { spawn(*argv, argv, envp, NULL); wait(); }
	else while (*envp) puts(*envp++);
}
