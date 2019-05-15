#include "sb.h"
#include "de.h"
#include <malloc.h>
#include <printk.h>
#include <h4/file/api.h>
#include <bug.h>
#include <panic.h>//

#if 0
static void printhex(unsigned char *dat, int len)
{
	int i;
	for (i = 0; i < len; i += 16) {
		printk("%06X:"
				" %02X %02X %02X %02X"
				" %02X %02X %02X %02X"
				" %02X %02X %02X %02X"
				" %02X %02X %02X %02X"
				,i,
				dat[i+0], dat[i+1], dat[i+2], dat[i+3],
				dat[i+4], dat[i+5], dat[i+6], dat[i+7],
				dat[i+8], dat[i+9], dat[i+10], dat[i+11],
				dat[i+12], dat[i+13], dat[i+14], dat[i+15],
				0);
	}
}
#endif

struct bpb
{
	char jmpboot[3];
	char oemname[8];
	uint16_t bps;
	uint8_t spc;
	uint16_t resv_secs;
	uint8_t num_fats;
	uint16_t root_ents;
	uint16_t tot_secs16;
	uint8_t media;
	uint16_t fat_secs16;
	uint16_t sec_per_track;
	uint16_t num_heads;
	uint32_t hid_secs;
	uint32_t tot_secs32;
} _PACKED;

struct bs16
{
	uint8_t drv_num;
	uint8_t reserved;
	uint8_t vol_id;
	uint32_t boot_sig;
	char vol_lab[11];
	char fs_type[8];
} _PACKED;

// https://github.com/archibate/OSASK/blob/master/src/haribote/file.c
static void fat_12to32(uint32_t *fat, uint8_t *img, size_t size)
/* ディスクイメージ内のFATの圧縮をとく */
{
	int i, j = 0;
	for (i = 0; i < size / 2; i += 2) {
		fat[i + 0] = (img[j + 0]      | img[j + 1] << 8) & 0xfff;
		fat[i + 1] = (img[j + 1] >> 4 | img[j + 2] << 4) & 0xfff;
		j += 3;
	}
	for (i = 0; i < size / 2; i++) {
		if (fat[i] >= 0xff0)
			fat[i] += 0xfffffff0 - 0xff0;
	}
	return;
}

sb_t *load_sb(int hd)
{
	struct bpb bpb;
	BUG_ON(pread(hd, &bpb, sizeof(bpb), 0) != sizeof(bpb));
	struct bs16 bs;
	BUG_ON(pread(hd, &bs, sizeof(bs), sizeof(bpb)) != sizeof(bs));

//#define PRINT_SB_ONLOAD 1
#ifdef PRINT_SB_ONLOAD
	printk("Loading a MS-DOS Volume:");
	printk("Filesys type: %s", bs.fs_type);
	printk("Volume Label: %s", bs.vol_lab);
	printk("Volume ID: %#x", bs.vol_id);
	printk("Drive No.: %#x", bs.drv_num);
	printk("OEMName: %s", bpb.oemname);
	printk("Hidden Secs: %d", bpb.hid_secs);
	printk("Media Type: %#x", bpb.media);
	printk("Reserved Secs: %d", bpb.resv_secs);
	printk("FAT12/16 Secs: %d", bpb.fat_secs16);
	printk("Sector size: %d", bpb.bps);
	printk("Sec per Cluster: %d", bpb.spc);
	printk("Total Secs: %d", bpb.tot_secs16 ? bpb.tot_secs16 : bpb.tot_secs32);
#else
	printk("fs: Loading MS-DOS Volume [%s]...", bs.vol_lab);
#endif

	sb_t *sb = malloc(sizeof(sb_t));
	sb->hd = hd;
	sb->bsize = bpb.bps * bpb.spc;
	sb->root_ents = bpb.root_ents;
	sb->rofs = false;

	sb->fat_base = bpb.bps * bpb.resv_secs;
	size_t fat_size = bpb.bps * bpb.fat_secs16;

	sb->root_beg = sb->fat_base + fat_size * bpb.num_fats;
	sb->begin = sb->root_beg + sb->root_ents * DESIZE - 2 * sb->bsize;//??
	sb->free_clus = 1;

	uint8_t *fat12 = malloc(fat_size);
	BUG_ON(pread(hd, fat12, fat_size, sb->fat_base) != fat_size);
	//printk("read fat at fat_base=%#x, fat_size=%#x", fat_base, fat_size);//
	fat_size = fat_size * 3 / 2;
	uint32_t *fat32 = malloc(fat_size);
	fat_12to32(fat32, fat12, fat_size);
	sb->fat = fat32;
	sb->fat_size = fat_size;
	//printhex(fat12, 100); BUG();
	free(fat12);

#ifdef PRINT_SB_ONLOAD
	sb_print(sb);
#endif
	return sb;
}

void sb_print(const sb_t *sb)
{
	printk("bsize %d", sb->bsize);
	printk("begin %#x", sb->begin);
	printk("root_beg %#x", sb->root_beg);
	printk("fat_size %#x", sb->fat_size);
	printk("root_ents %d", sb->root_ents);
}

void sb_free(sb_t *sb)
{
	free(sb->fat);
	free(sb);
}
