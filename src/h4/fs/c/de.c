#include "de.h"
#include <string.h>
#include <panic.h>
#include <lohitools.h>
#include <numtools.h>
#include <errno.h>
#include <bug.h>
#include <printk.h>//

uint32_t egetclus(const de_t *e)
{
	return MKDWORD(e->clus_lo, e->clus_hi);
}

void esetclus(de_t *e, uint32_t clus)
{
	e->clus_lo = LOWORD(clus);
	e->clus_hi = HIWORD(clus);
}

void egetname(const de_t *e, char *buf)
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

int esetname(const de_t *e, const char *name)
{
	char *p;
	size_t len, lena;
	const char *n, *na = name;
	memset(e->name, ' ', sizeof(e->name));
	memset(e->ext, ' ', sizeof(e->ext));
	while (n = strchr(na, '.'))
		na = n + 1;
	if (na != name) {
		len = na - 1 - name;
		lena = strlen(na);
		if (lena > sizeof(e->ext))
			return -ENAMETOOLONG;
		memcpy(e->ext, na, lena);
	} else {
		len = strlen(name);
	}
	if (len > sizeof(e->name))
		return -ENAMETOOLONG;
	memcpy(e->name, name, len);
	for (p = &e->name[0]; p < &e->name[8]; p++) {
		if ('a' <= *p && *p <= 'z')
			*p -= 'a' - 'A';
	}
	for (p = &e->ext[0]; p < &e->ext[3]; p++) {
		if ('a' <= *p && *p <= 'z')
			*p -= 'a' - 'A';
	}
	return 0;
}

int ecmpname(const de_t *e, const char *name)
{
	char ent_name[NAME_MAX+1];
	egetname(e, ent_name);
	//if (e->name[0]) printk("ecmpname: [%s] vs [%s] = %d", name, ent_name, stricmp(name, ent_name));
	return stricmp(ent_name, name);
}
