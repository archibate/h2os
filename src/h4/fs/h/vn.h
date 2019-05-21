#pragma once

#include "de.h"
#include "sb.h"
#include <h4/fs/oflags.h>
#include <list.h>

#define CBUFMAX 4

typedef struct virfile
{
	struct hlist_head cache_lru;
	enum {
		VN_REGFAT,
		VN_ROOTDIR,
	} type;
	unsigned int refcount;
	//int cache_id;
	size_t size;
	uint32_t clus_start;
	uint32_t attr;
	sb_t *sb;
	uint32_t exflags;
	off_t lastpos;
	off_t dehdoff;
} vn_t;

ssize_t vread(vn_t *v, void *buf, size_t len, off_t off);
ssize_t vwrite(vn_t *v, const void *buf, size_t len, off_t off);
vn_t *__vopen(sb_t *sb, de_t *e);
vn_t *vopenfile(sb_t *sb, de_t *e);
vn_t *vopendir(sb_t *sb, de_t *e);
vn_t *vdup(vn_t *v);
void vupdate(vn_t *v);
int vdeunlink(vn_t *v);
int vclose(vn_t *v);
