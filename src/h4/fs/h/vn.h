#pragma once

#include "de.h"
#include "sb.h"
#include <h4/fs/oflags.h>

#define CBUFMAX 4

typedef struct virfile
{
	uint32_t clus[CBUFMAX];
	uint32_t blkn[CBUFMAX];
	uint32_t attr;
	size_t size;
	sb_t *sb;
	uint32_t exflags;
} vn_t;

ssize_t vread(vn_t *v, void *buf, size_t len, off_t off);
ssize_t vwrite(vn_t *v, const void *buf, size_t len, off_t off);
vn_t *__vopen(sb_t *sb, de_t *e);
vn_t *vopenfile(sb_t *sb, de_t *e);
vn_t *vopendir(sb_t *sb, de_t *e);
int vclose(vn_t *v);
