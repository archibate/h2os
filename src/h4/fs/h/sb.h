#pragma once

typedef struct superblk
{
	int hd;
	size_t bsize;
	size_t begin;
	uint32_t *fat;
	bool rofs;
	size_t fat_size;
	size_t root_ents;
} sb_t;

sb_t *load_sb(int hd);
void sb_print(const sb_t *sb);
void sb_free(sb_t *sb);
struct virfile *sb_openroot(sb_t *sb);
