#include <h4/fs.h>
#include <h4/file.h>
#include <stdio.h>

int main(void)
{
	close(0);
	close(1);
	close(2);
	open("/dev/keybd", O_RDONLY);
	open("/dev/cons", O_WRONLY);
	dup(1);

	printf("init: started\n");
	return 0;
}
