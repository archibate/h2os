#include <stdio.h>
#include <string.h>

static int width = 16;
static int i = 0, newline = 1;

void hex(const char *path)
{
	FILE *fp = !strcmp(path, "-") ? stdin : fopen(path, "r");
	if (!fp) { perror(path); return; }
	char buf[1024];

	unsigned int ch;
	while (EOF != (ch = fgetc(fp))) {
		if (newline)
			printf("%04X: ", i);
		newline = !(++i % width);
		printf("%02X%c", ch, " \n"[newline]);
	}
	if (ferror(fp)) perror(path);

	if (fp != stdin)
		fclose(fp);
}

int main(int argc, char **argv)
{
	if (!*++argv) hex("-");
	else while (*argv) hex(*argv++);
	putchar('\n');
	return 0;
}
