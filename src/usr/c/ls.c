#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <memory.h>

// {{{
#include <lohitools.h>
uint32_t egetclus(const struct dirent *e)
{
	return MKDWORD(e->clus_lo, e->clus_hi);
}

#define NAME_MAX 12
void egetname(const struct dirent *e, char *buf)
{
#define ISBLANK(c) (strchr(" \t\n\r", (c)))

	int i;
	for (i = 0; i < 8; i++) {
		if (ISBLANK(e->name[i]))
			break;
		*buf++ = e->name[i];
	}

	if (ISBLANK(e->ext[0]))
		goto out;

	*buf++ = '.';
	*buf++ = e->ext[0];

	for (i = 1; i < 3; i++) {
		if (ISBLANK(e->ext[i]))
			break;
		*buf++ = e->ext[i];
	}

out:	*buf++ = 0;

	return;
}
// }}}

void showde(struct dirent *de)
{
	char name[NAME_MAX+1];
	char attr[] = "?---";
	egetname(de, name);
	if (!name[0]) return;
	if (de->attr & T_VOL) attr[0] = 'v';
	else if (de->attr & T_DIR) attr[0] = 'd';
	else if (de->attr & T_REG) attr[0] = '-';
	else if (de->attr & T_LNK) attr[0] = 'l';
	else if (de->attr & T_NOD) attr[0] = 'n';
	if (de->attr & T_RO ) attr[1] = 'r';
	if (de->attr & T_HID) attr[2] = 'i';
	if (de->attr & T_SYS) attr[3] = 's';
	printf("%s  %s\n", attr, name);
}

void ls(const char *path)
{
	FILE *fp = fopen(path, "d");
	if (!fp) { perror(path); goto out; }
	struct dirent de;
	while (fread(&de, sizeof(de), 1, fp) == 1)
		showde(&de);
out:	fclose(fp);
}

int main(int argc, char **argv)
{
	if (*++argv) while (*argv) ls(*argv++);
	else ls(".");
	return 0;
}
