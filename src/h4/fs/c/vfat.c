// http://blog.chinaunix.net/uid-23141914-id-211775.html
#include "vn.h"
#include "sb.h"
#include "vn.h"
#include "de.h"
#include <errno.h>
#include <malloc.h>
#include <numtools.h>
#include <printk.h>
#include <stddef.h>
#include <h4/file/api.h>
#include "error.h"
#include <bug.h>
#include <panic.h>
#include <list.h>

#if 0
struct fat_cache
{
	struct hlist_head hlist;
	int nr_config;
	int fcluster;
	int dculster;
};

static void fat_cache_update_lru(vn_t *v, struct fat_cache *hit)
{
	hlist_move(&hit->hlist, &v->cache_lru);
}

static int fat_cache_lookup(vn_t *v, int fclus)
{
	static struct fat_cache nohit = { .fcluster = 0, };
	struct fat_cache *hit = &nohit, *p;
	int offset = -1;

	hlist_for_each_entry2(p, &v->cache_lru, hlist) {
		/* find the cache of `fclus` or nearest cached */
		if (p->fcluster <= fclus && hit->fcluster < p->fcluster) {
			hit = p;
			if ((hit->fcluster + hit->nr_config) < fclus) {
				offset = hit->nr_config;
			} else {
				offset = fclus - hit->fcluster;
				break;
			}
		}
	}

	if (hit != &nohit) {
		fat_cache_update_lru(vn, hit);
		/*cid->id = vn->cache_id;
		cid->nr_config = hit->nr_config;
		cid->fcluster = hit->fcluster;
		cid->dcluster = hit->dcluster;
		*cached_fclus = cid->fcluster + offset;
		*cached_dclus = cid->dcluster + offset;*/
	}

	return offset;
}

int fat_get_cluster(vn_t *vn, int cluster, int *fclus, int *dclus)
{
	sb_t *sb = vn->sb;
	int nr = -EIO;

	*fclus = 0;
	*dclus = vn->clus_start;

	if (cluster == 0)
		return 0;

	if (fat_cache_lookup(vn, cluster, &cid, fclus, dclus) < 0) {
		/*
		 * dummy, always not contiguous
		 * This is reinitialized by cache_init(), later.
		 */
		cache_init(&cid, -1, -1);
	}

	while (*fclus < cluster) {
		/* prevent the infinite loop of cluster chain */
		if (*fclus > limit) {
			printk(KL_ERROR "detected cluster chain loop");
			nr = -EIO;
			goto out;
		}

		nr = sb->fat[*dclus];
		if (nr < 0) {
			printk(KL_WARN "bad cluster number %#x(%d)", nr, nr);
			nr = -EIO;
			goto out;
		}

		(*fclus)++;
		*dclus = nr;
		if (!cache_contiguous(&cid, *dclus)) {
			cache_init(&cid, *fclus, *dclus);
		}
	}
	nr = 0;
	fat_cache_add(vn, &cid);

out:
	return nr;
}
#endif

static void sb_update_fat_at(sb_t *sb, uint32_t cl)
{
	uint32_t c = sb->fat[cl];
	uint8_t buf[3];
	off_t base = sb->fat_base + (cl / 2) * 3;
	if (c >= 0xfffffff0)
		c -= 0xfffffff0 - 0xff0;
	BUG_ON(pread(sb->hd, &buf, sizeof(buf), base) != sizeof(buf));
	if (cl % 2 == 0) {
		buf[0] = c & 0xff;
		buf[1] &= 0xf0;
		buf[1] |= (c >> 8) & 0x0f;
	} else {
		buf[1] &= 0x0f;
		buf[1] |= (c << 4) & 0xf0;
		buf[2] = (c >> 4) & 0xff;
	}
	BUG_ON(pwrite(sb->hd, &buf, sizeof(buf), base) != sizeof(buf));
}

static uint32_t fat_sb_allocate_cluster(sb_t *sb)
{
	uint32_t cl;
	BUG_ON(!sb->free_clus);
	for (cl = sb->free_clus; cl < sb->fat_size / sizeof(uint32_t); cl++) {
		if (sb->fat[cl] == 0) {
			sb->fat[cl] = 0xfffffffb;
			sb_update_fat_at(sb, cl);
			return cl;
		}
	}
	panic("fs: disk out of cluster");
}

static uint32_t sb_kick_cluster(sb_t *sb, uint32_t *pclus)
{
	uint32_t cl = *pclus;
	switch (cl) {
		case 0x00000000:
		case 0xfffffff7:
			return 0;
	}
	if (cl >= 0xfffffff8) {
		cl = fat_sb_allocate_cluster(sb);
		printk("!!!Doing EOF cluster extend to %d(experimental)", cl);
		*pclus = cl;
	}
	return cl;
}

static uint32_t sb_return_cluster(sb_t *sb, uint32_t *pclus)
{
	uint32_t cl = *pclus;
	if (cl >= 0xfffffff7)
		return 0;
	return cl;
}

ssize_t __vrw_regfat(vn_t *v, void *buf, size_t len, off_t off, bool wr)
{
	bool update = false;
	uint32_t (*kicker)(sb_t *, uint32_t *) = wr ? sb_kick_cluster : sb_return_cluster;
	//printk("fat_v%s(%p, %d, %d)", wr ? "write" : "read", buf, len, off);
	sb_t *sb = v->sb;
	size_t bsize = sb->bsize;
	uint32_t clus_start = v->clus_start;
	uint32_t clus = kicker(sb, &clus_start);
	if (!clus)
		return -EIO;
	if (v->clus_start != clus_start) {
		v->clus_start = clus_start;
		update = true;
	}

	if (off > v->size) {
		printk(KL_WARN "vread/vwrite: off > v->size: invalid offset");
		return -EINVAL;
	}
	//if (wr) { printk("vwrite[%s](%d, %d)", buf, len, off); }
	if (off + len > v->size) {
		if (wr) {
			//printk("vwrite: extend %d to %d", v->size, off + len);
			v->size = off + len;
			update = true;
		} else {
			len = v->size - off;
		}
	}

	if (update)
		vupdate(v);

		//if (1||clus >= 0xff0) printk("clus=%d", clus);
	for (; off >= sb->bsize; off -= sb->bsize) {
		clus = kicker(sb, &sb->fat[clus]);
		if (!clus)
			return -EIO;
	}

	//printk("clus=%d", clus);

	size_t m, n = len;
	if (n > 0)
		goto go;
	while (n > 0) {
		clus = sb->fat[clus];
		if (clus >= 0xfffffff0)
			return -EIO;
go:
		m = n;
		CLMAX(m, bsize - off);

		v->lastpos = v->sb->begin + bsize * clus + off;

		if (wr)
			pwrite(v->sb->hd, buf, m, v->lastpos);
		else
			pread(v->sb->hd, buf, m, v->lastpos);

		n -= m;
		buf += m;
		off = 0;
	}

	return len - n;
}
