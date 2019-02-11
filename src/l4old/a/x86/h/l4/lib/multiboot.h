#pragma once

#include <inttypes.h>

#define MB_MAGIC     0x1badb002
#define MB_MAGIC_EAX 0x2badb002

#define MBH_PGALIGN   (1<<0)
#define MBH_MEMINFO   (1<<1)
#define MBH_VIDEO     (1<<2)

struct mboot_header
{
	uint magic;
	uint flags;
	uint checksum;
};

#define MAKE_MBOOT_HEADER(flags) \
  MB_MAGIC, (flags), -(MB_MAGIC + (flags))

#define MBI_MEMINFO (1<<0)
#define MBI_BOOTDEV (1<<1)
#define MBI_CMDLINE (1<<2)
#define MBI_MODULES (1<<3)

struct mboot_info
{
	uint flags;
	uint mem_lower;
	uint mem_upper;
	uint boot_device;
	uint cmdline;
	uint mods_count;
	uint mods_addr;
};

struct mboot_module
{
	uint mod_start;
	uint mod_end;
	uint mod_string;
	uint reserved;
};
