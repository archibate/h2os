#include <stdio.h>

int main(int argc, char **argv)
{
	while (*++argv)
		printf("%s%c", *argv, argv[1] ? ' ' : '\n');
	return 0;
}
