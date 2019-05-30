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

void printdostime(uint16_t time)
{
	struct dos_ts {
		uint16_t sec2 : 5;
		uint16_t min : 6;
		uint16_t hour : 5;
	} *t = (void*)&time;
	//printf("%02d:%02d:%02d", t->hour, t->min, t->sec2 * 2);
	printf("%02d:%02d", t->hour, t->min);
}

void printdosdate(uint16_t date)
{
	struct dos_ds {
		uint16_t day : 5;
		uint16_t mon : 4;
		uint16_t year1980 : 7;
	} *d = (void*)&date;
	//printf("%04d/%02d/%02d", d->year1980 + 1980, d->mon, d->day);
	printf("%02d/%02d", d->mon, d->day);
}

void showde(struct dirent *de)
{
#if 0
	const char *scale = "BKMG";
	char name[NAME_MAX+1];
	char attr[] = "?---";
	size_t size = de->size;
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
	while (size >= 2*4096 && *scale)
		size /= 4096, scale++;
	printf("%s % 5.5d ", attr, egetclus(de));
#if 0
	printdosdate(de->cdate);
	printf(" ");
	printdostime(de->ctime);
#endif
	printf(" % 4d%c %s%s", size, *scale, name, attr[0]=='d'?"/\n":"\n");
#else
	char name[NAME_MAX+1];
	egetname(de, name);
	if (!name[0]) return;
	puts(name);
#endif
}

void ls(const char *path)
{
	FILE *fp = fopen(path, "rd");
	if (!fp) { perror(path); return; }
	struct dirent de;
	while (fread(&de, sizeof(de), 1, fp) == 1)
		showde(&de);
	if (ferror(fp)) perror(path);
	fclose(fp);
}

int main(int argc, char **argv)
{
	if (*++argv) while (*argv) ls(*argv++);
	else ls(".");
	return 0;
}
