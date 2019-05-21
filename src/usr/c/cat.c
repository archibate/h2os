#include <stdio.h>
#include <string.h>

void cat(const char *path)
{
	FILE *fp = !strcmp(path, "-") ? stdin : fopen(path, "r");
	if (!fp) { perror(path); return; }
	char buf[1024];
	while (fgets(buf, sizeof(buf), fp))
		fputs(buf, stdout);
	if (ferror(fp)) perror(path);
	if (fp != stdin)
		fclose(fp);
}

int main(int argc, char **argv)
{
	if (!*++argv) cat("-");
	else while (*argv) cat(*argv++);
	return 0;
}
