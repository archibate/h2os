// http://blog.chinaunix.net/uid-23141914-id-211775.html
#include "vn.h"
#include "sb.h"
#include "vn.h"
#include "de.h"
#include <errno.h>
#include <malloc.h>
#include <numtools.h>
#include <printk.h>
#include <h4/file/api.h>
#include "error.h"
#include <bug.h>
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

ssize_t __vrw_regfat(vn_t *v, void *buf, size_t len, off_t off, bool wr)
{
	//printk("fat_v%s(%p, %d, %d)", wr ? "write" : "read", buf, len, off);
	sb_t *sb = v->sb;
	size_t bsize = sb->bsize;
	uint32_t clus = v->clus_start;

	if (off + len > v->size) {
		printk(KL_WARN "off + len > v->size: notsup, returning EINVAL");
		return -EINVAL;
	}

	for (; off >= sb->bsize; off -= sb->bsize) {
		//printk("clus=%d", clus);
		clus = sb->fat[clus];
		if (clus >= 0xfffffff0)
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
		off_t cbase = v->sb->begin + bsize * clus;

		//printk("cbase=%#x", cbase);

		if (wr)
			pwrite(v->sb->hd, buf, m, cbase + off);
		else
			pread(v->sb->hd, buf, m, cbase + off);

		n -= m;
		buf += m;
		off = 0;
	}

	return len - n;
}
