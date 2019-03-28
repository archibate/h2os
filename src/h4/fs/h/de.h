#pragma once

// dirent.attr
#define T_RO     (1<<0)
#define T_HID    (1<<1)
#define T_SYS    (1<<2)
#define T_VOL    (1<<3)
#define T_NOD    (1<<4)
#define T_DIR    (1<<5)
#define T_REG    (1<<6)
#define T_LNK    (1<<7)

typedef struct dirent
{
	char name[8];
	char ext[3];
	uint8_t attr;
	uint8_t reserved;
	uint8_t ctime_10ms;
	uint16_t ctime;
	uint16_t cdate;
	uint16_t adate;
	uint16_t clus_hi;
	uint16_t mtime;
	uint16_t mdate;
	uint16_t clus_lo;
	uint32_t size;
} _PACKED de_t;
#define DESIZE sizeof(de_t)

uint32_t egetclus(const de_t *e);
void esetclus(de_t *e, uint32_t clus);
int ecmpname(const de_t *e, const char *name);
void egetname(const de_t *e, char *buf);
