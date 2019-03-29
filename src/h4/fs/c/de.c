#include "de.h"
#include <string.h>
#include <panic.h>
#include <lohitools.h>
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

int ecmpname(const de_t *e, const char *name)
{
	char ent_name[NAME_MAX+1];
	egetname(e, ent_name);
	return stricmp(ent_name, name);
}
