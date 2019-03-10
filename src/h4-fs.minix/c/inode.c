// https://github.com/archibate/OS67/blob/master/fs/inode.c
#include "minix.h"
#include "inode.h"
#include "stat.h"
#include "sb.h"
#include <printk.h>
#include <stddef.h>
#include <assert.h>
#include <memory.h>
#include <h4/proc.h>
#include <numtools.h>

#define NICACHE 256
/* inodes cache */
static struct inode icache[NICACHE];

/* find the inode with specific inum in memory,
 * if not found, use first empty inode
 * 注意: 这里只会申请一个可用的 inode 槽位, 增加一个内存引用, 不会锁住, 也不会从磁盘读出 */
struct inode* iget(ino_t ino)
{
	struct inode *ip, *empty;

	empty = NULL;
	for (ip = &icache[0]; ip < &icache[NICACHE]; ip++) {
		/* this inode is cached */
		if (ip->ref > 0 && ip->ino == ino) {
			printk("iget: inode-%d cached", ino);
			ip->ref++;
			return ip;
		}
		/* remember the first free inode */
		if (!empty && ip->ref == 0) {
			empty = ip;
		}
	}
	assert_info(empty, "iget: no inode");

	printk("iget: inode-%d not cached, found a empty one", ino);

	ip = empty;
	ip->ino   = ino;
	ip->ref   = 1;
	ip->flags = 0;
	return ip;
}

/* alloc a inode with specific type
 * - 磁盘: 在 inode bitmap 中寻找一个空闲的位, 清空该位对应的 d_inode
 * - 内存: 通过 iget() 在内存中申请一个对应的 inodes 槽位
 */
struct inode* ialloc(void)
{
	int ino;
	struct inode *ip;

	ino = _ialloc();
	ip  = iget(ino);

	ip->nlinks = 0;
	ip->size   = 0;
	ip->mode   = S_RWX;
	memset(ip->zone, 0, sizeof(ip->zone));
	return ip;
}

/* copy a in-memory inode to disk */
void iupdate(struct inode *ip)
{
	struct buf *bp;
	struct d_inode *d_ip;
	struct super_block sb;

	printk("iupdate: updata inode-%d", ip->ino);

	read_sb(&sb);

	bp = bread(IBLK(sb, ip->ino));
	d_ip = (struct d_inode *)bp->data + (ip->ino-1)%IPB;

	/* ip -> d_ip */
	d_ip->mode   = ip->mode;
	d_ip->uid    = ip->uid;
	d_ip->size   = ip->size;
	d_ip->mtime  = ip->mtime;
	d_ip->gid    = ip->gid;
	d_ip->nlinks = ip->nlinks;
	memcpy(d_ip->zone, ip->zone, sizeof(ip->zone));

	bwrite(bp);
	brelse(bp);
}

/* truncate inode
 * only called when the inode has no links to it
 * and not any in-memory ref to it
 * - 在磁盘上没有被任何目录引用 nlinks = 0
 * - 在内存里没有引用(没有被打开) ref = 0
 */
static void itrunc(struct inode *ip)
{
	uint32_t i, j;
	struct buf *bp;
	uint16_t *zone2;

	printk("itrunc: truncate inode-%d", ip->ino);

	printk("itrunc:  direct block");

	/* free DIRECT block */
	for (i = 0; i < NDIRECT; i++) {
		if (ip->zone[i]) {
			bfree(ip->zone[i]);
			ip->zone[i] = 0;
		}
	}

	/* free INDIRECT block */
	if (ip->zone[NDIRECT]) {
		printk("itrunc:  indirect block");

		bp = bread(ip->zone[NDIRECT]);
		zone2 = (uint16_t *)bp->data;
		for (j = 0; j < NINDIRECT; j++) {
			if (zone2[j]) {
				bfree(zone2[j]);
				zone2[j] = 0;
			}
		}
		brelse(bp);
		bfree(ip->zone[NDIRECT]);
		ip->zone[NDIRECT] = 0;
	}

	/* DAUL INDIRECT unused */

	ip->size = 0;
	iupdate(ip);
}

/* reference of ip + 1 */
struct inode* idup(struct inode *ip)
{
	ip->ref++;
	printk("idup: inode-%d's ref: %d", ip->ino, ip->ref);
	return ip;
}

/* reference of ip - 1
 * if it is the last ref, this node can be recycle (ref = 1) (?)
 * and if this inode have no link to it (nlinks = 0)
 * free this inode in the disk
 */
void iput(struct inode *ip)
{
	printk("iput: inode-%d's ref: %d", ip->ino, ip->ref);

	if (ip->ref == 1 && (ip->flags & I_VALID) && ip->nlinks == 0) {
		printk("iput: free inode-%d", ip->ino);

		/* can not free a locked inode */
		assert(!(ip->flags & I_BUSY), "iput: free a locked inode");
		ip->flags |= I_BUSY;
		itrunc(ip);

		_ifree(ip->ino);

		// ip-> ref == 0
		// ip->flags = 0;
	}
	ip->ref--;
}

/* lock given inode with flag I_BUSY
 * read the inode form disk if necessary
 * non-ref inode can not be lock (ref = 0)
 */
void ilock(struct inode *ip)
{
	struct buf *bp;
	struct d_inode *d_ip;
	struct super_block sb;

	printk("ilock: lock inode-%d", ip->ino);

	assert_info(ip, "ilock: null pointer");
	assert_info(ip->ref, "ilock: lock a noo-ref inode");

	read_sb(&sb);

	// TODO: maybe use sleep()/wakeup()
	int timeout = 20000;
	while (ip->flags & I_BUSY) {
		pause();
		assert_info(timeout--, "ilock: wait for a busy inode & out of time")
	}
	ip->flags |= I_BUSY;

	/* need to be read form disk */
	if (!(ip->flags & I_VALID)) {
		bp = bread(IBLK(sb, ip->ino));
		d_ip = (struct d_inode *)bp->data + (ip->ino-1)%IPB;

		/* ip -> dip */
		ip->mode = d_ip->mode;
		ip->uid = d_ip->uid;
		ip->size = d_ip->size;
		ip->mtime = d_ip->mtime;
		ip->gid = d_ip->gid;
		ip->nlinks = d_ip->nlinks;

		memcpy(ip->zone, d_ip->zone, sizeof(d_ip->zone));

		brelse(bp);
		ip->flags |= I_VALID;
		//assert_info(ip->type, "ilock: inode no type");
	}
}

/* unlock a inode */
void iunlock(struct inode *ip) {
	printk("iunlock: unlock inode-%d", ip->ino);

	assert_info(ip, "iunlock: null pointer");
	assert_info(ip->ref, "iunlock: unlock a noo-ref inode"); // (?)
	assert_info(ip->flags & I_BUSY, "iunlock: unlock a unlocked inode");

	ip->flags &= ~I_BUSY;
}



// (?)
void iunlockput(struct inode *ip) {
	iunlock(ip);
	iput(ip);
}

/* return the disk block address of the nth block in given inode
 * if not such  block, alloc one
 */
static uint16_t bmap(struct inode *ip, blkno_t bn) {
	uint16_t zno;
	uint16_t *zone2;
	struct buf *bp;

	assert(bn < NDIRECT + NINDIRECT,"bmap: out of range");

	if (bn < NDIRECT) {
		if (!(zno = ip->zone[bn])) {
			zno = ip->zone[bn] = balloc();
			printk("bmap: zone-%d no exist, alloc blk-%d", bn, zno);
		}
		printk("bmap: inode-%d zone-%d, return direct blk-%d", ip->ino, bn, zno);
		return zno;
	}

	bn -= NDIRECT;

	if ((ip->zone[NDIRECT]) == 0) {
		ip->zone[NDIRECT] = balloc();
		printk("bmap: INDIRECT zones no exist, alloc blk-%d", ip->zone[NDIRECT]);
	}

	printk("bmap: read INDIRECT zones table blk-%d", ip->zone[NDIRECT]);

	bp = bread(ip->zone[NDIRECT]);
	zone2 = (uint16_t*)bp->data;

	if ((zno = zone2[bn]) == 0) {
		zno = zone2[bn] = balloc();
		bwrite(bp);
		printk("bmap: zone-%d no exist, alloc INDIRECT blk-%d", bn + NDIRECT, zno);
	}

	brelse(bp);
	printk("bmap: inode-%d, zone-%d, return INDIRECT blk-%d", ip->ino, bn + NDIRECT, zno);
	return zno;
}

/* read data from inode */
int iread(struct inode *ip, void *buf, size_t len, off_t off)
{
	size_t tot, m;
	struct buf *bp;

	/* is a char or block device? */
	if (S_ISCHR(ip->mode) || S_ISBLK(ip->mode)) {
		printk("iread: inode-%d is device %d, forwarding", ip->ino, ip->zone[0]);
		return pread(dtable[ip->zone[0]], buf, len, off);
	}

	/* 偏移溢出 */
	if (off + len < off) {
		printk(KL_WARN "iread: overflow offset + length");
		return -E2BIG;
	}
	if (off > ip->size) {
		printk(KL_ERROR "iread: offset out of file size (ENOTSUP)");
		return -ENOTSUP;
	}
	if (off + len > ip->size) {
		printk(KL_WARN "iread: off + n > size, ignore");
		len = ip->size - off;
	}

	printk("iread: inode-%d offset: 0x%x read: 0x%x", ip->ino, off, len);
	/*  tot: 已读取的字节数
	 *  n: 要读取的字节数
	 *  off: 当前文件指针偏移
	 *  m: 当次循环要读取的字节数
	 */
	for (tot = 0; tot < n; tot += m, off += m, buf += m) {
		bp = bread(bmap(ip, off/BSIZE));
		/* 每个循环读取的数据可分两种情况：
		 * - 从当前文件偏移读到该扇区结束
		 * - 从当前文件偏移读到 *满足要读取的字节数*
		 * 每次选择较小的那个
		 */
		m = MIN(len - tot, BSIZE - off%BSIZE);
		/* 从当前偏移相对于该扇区的偏移处起开始读取
		 * (在中间的循环这个值常常是0)
		 */
		printk("iread: read off 0x%x len %d", off%BSIZE, m);
		memcpy(buf, bp->data + off%BSIZE, m);
		brelse(bp);
	}

	return len;
}

/* write data to inode */
int iwrite(struct inode *ip, const void *buf, size_t len, off_t off)
{
	size_t tot, m;
	struct buf *bp;

	/* is a char or block device? */
	if (S_ISCHR(ip->mode) || S_ISBLK(ip->mode)) {
		printk("iwrite: inode-%d is device %d, forwarding", ip->ino, ip->zone[0]);
		return pwrite(dtable[ip->zone[0]], buf, len, off);
	}

	printk("iwrite: inode-%d offset: 0x%x len:a0x%x", ip->ino, off, n);

	if (off + len < off) {
		printk(KL_WARN "iwrite: overflow offset + length");
		return -E2BIG;
	}
	if (off + len > MAXFILEBLK*BSIZE) {
		printk(KL_WARN "iwrite: file too big");
		return -E2BIG;
	}
	if (off > ip->size) {
		printk(KL_ERROR "iwrite: offset out of file size (ENOTSUP)");
		return -ENOTSUP;
	}

	for (tot = 0; tot < len; tot += m, src += m, off += m) {
		bp = bread(bmap(ip, off/BSIZE));
		m = MIN(n - tot, BSIZE - off%BSIZE);
		memcpy(bp->data + off%BSIZE, src, m);
		bwrite(bp);
		brelse(bp);
	}

	/* if alloc new block to ip, update it's size */
	// len > 0 ?
	if (len > 0 && off > ip->size) {
		ip->size = off;
		iupdate(ip);
	}

	return len;
}

static char *strmode(mode_t mode)
{
	char s[] = "----------";
	switch (mode & S_IFMT) {
	case S_IFREG: s[0] = '-'; break;
	case S_IFDIR: s[0] = 'd'; break;
	case S_IFLNK: s[0] = 'l'; break;
	case S_IFBLK: s[0] = 'b'; break;
	case S_IFCHR: s[0] = 'c'; break;
	case S_IFIFO: s[0] = 'f'; break;
	default:      s[0] = '?'; break;
	}
	int i;
	for (i = 0; i < 9; i++) {
		if (mode & 0400)
			s[1+i] = "rwx"[i % 3];
		mode <<= 1;
	}
	return s;
}

void print_i(struct inode *ip)
{
	printk("print_i %#p", ip);
	printk("  ino: %d", ip->ino);
	printk("  ref: %d", ip->ref);
	printk("  flags: %c%c",
			(ip->flags & I_BUSY)  ? 'b' : '-',
			(ip->flags & I_VALID) ? 'v' : '-');

	printk("  mode:   %s", strmode(ip->mode));
	printk("  size:   %d", ip->size);
	printk("  nlinks: %d", ip->nlinks);

	printk("  zones:");

	int i;
	for (i = 0; i < NDIRECT; i++)
		if (ip->zone[i])
			printk("     blk-%d", ip->zone[i]);
	}
}

void istat(struct inode *ip, struct stat *st)
{
	st->dev  = ROOT_DEV;
	st->mode = ip->mode;
	st->ino  = ip->ino;
	st->size = ip->size;
}
