#include <stdio.h>
#include <string.h>

void cat(const char *s)
{
	FILE *fp = !strcmp(s, "-") ? stdin : fopen(s, "r");
	if (!fp) { perror(s); return; }
	char buf[1024];
	while (fgets(buf, sizeof(buf), fp))
		fputs(buf, stdout);
	if (fp != stdin)
		fclose(fp);
}

int main(int argc, char **argv)
{
	if (!*++argv) cat("-");
	else while (*argv) cat(*argv++);
	return 0;
}
