#include <spawn.h>

int main(int argc, char **argv)
{
	spawn(argv[0], argv, NULL, NULL);
	wait();
	return 0;
}
