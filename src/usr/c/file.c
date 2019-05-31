#include <stdio.h>

char C(char c)
{
	return (c >= '.' && c < 0x7f) ? c : '.';
}

int main(int argc, char **argv)
{
	unsigned char buf[4];
	if (!argv[1]) {
		fprintf(stderr, "file: bad argument\n");
		return 1;
	}
	FILE *fp = fopen(argv[1], "rb");
	if (!fp || fread(buf, sizeof(buf), 1, fp) != 1)
		perror(argv[1]);
	else {
		printf("%02X %02X %02X %02X %c%c%c%c\n",
				buf[0], buf[1], buf[2], buf[3],
				C(buf[0]), C(buf[1]), C(buf[2]), C(buf[3]));
	}
	fclose(fp);
}
