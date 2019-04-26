#include <h4/fs.h>
#include <h4/file.h>
#include <stdio.h>

int main(void)
{
	int fd = open("/dev/cons", O_WRONLY);
	FILE *fp = fdopen(fd, "w");
	fprintf(fp, "Hello, World!\n");
	return 0;
}
