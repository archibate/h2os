// https://github.com/archibate/OS67/blob/master/inc/minix.h
#pragma once

#include <inttypes.h>
#include <h4/fs/types.h>
#include <h4/blk/types.h>
#include <h4/blk/defines.h>

struct super_block
{
	uint16_t ninodes;       // number of inodes
	uint16_t nzones;        // number of zones
	uint16_t imap_blk;      // space uesd by inode map (block)
	uint16_t zmap_blk;      // space used by zone map  (block)
	uint16_t fst_data_zone; // first zone with `file` data
	uint16_t log_zone_size; // size of a data zone = 1024 << log_zone_size

	uint32_t max_size;      // max file size (byte)
	uint16_t magic;         // magic number
	uint16_t state;         // (?) mount state
};

/* in-disk minix inode */
struct d_inode
{
	uint16_t mode;  // file type and RWX(unused) bit
	uint16_t uid;   // identifies the user who owns the file (unused)
	uint32_t size;  // file size (byte)
	uint32_t mtime; // time since Jan. 1st, 1970 (second) (unused)
	uint8_t gid;    // owner's group (unused)
	uint8_t nlinks; // number of dircetory link to it

	/* zone[0] - zone[6] point to to direct blocks
	 * - for indoe which is virtual device, zone[0] used to store device index
	 * zone[7] points to a indirect block table
	 * zone[8]  points to a double indirect block table (unused)
	 * so this file system's real max file size = (7 + 512) * 1024 byte = 519 kb
	 */
	uint16_t zone[9];
};


/* inode status flag */
#define I_BUSY 0x1
#define I_VALID 0x2

/* in-memorty inode */
struct inode
{
	uint16_t dev;   // be 0 forever
	uint32_t ino;
	uint16_t ref;
	uint16_t flags;
	uint16_t atime; // (unused)
	uint16_t ctime; // (unused)

	// struct d_inode {
	uint16_t mode;
	uint16_t uid;   // (unused)
	uint32_t size;
	uint32_t mtime; // (unused)
	uint8_t gid;    // (unused)
	uint8_t nlinks;
	uint16_t zone[9];
	//}
};

#define NDIRECT 7 // 一个 i 节点直接管辖的块数目
#define NINDIRECT (BSIZE/sizeof(uint16_t))  // 1024/2 = 512
#define NDUAL_INDIRECT (BSIZE*BSIZE/sizeof(uint16_t)) // (unused)
#define MAXFILE (NDIRECT + NINDIRECT)       // 512 + 7 kb

#define NAME_LEN 14
/* minix directroy entry */
struct dir_entry
{
	uint16_t ino;
	char name[NAME_LEN];
};

/* block contain inode i
 * NB: inode number starts at 1 */
#define IBLK(sb, i) (2 + (sb).imap_blk + (sb).zmap_blk + ((i)-1)/IPB)

/* bitmap contain inode i */
#define IMAP_BLK(sb, i) (2 + ((i)-1)/BPB)
/* bitmap contain block z */
#define ZMAP_BLK(sb, b) (2 + (sb).imap_blk + (b)/BPB)

/* minix fs methods */
void read_sb(int hd, struct super_block *sb);
