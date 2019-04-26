#pragma once

#include <inttypes.h>

// https://github.com/archibate/jos/blob/master/lab1/code/inc/elf.h

#define ELF_MAGIC 0x464C457FU	/* "\x7FELF" in little endian */

struct Elfhdr {
	uint32_t e_magic;	// must be ELF_MAGIC
	uint8_t e_ident[12];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
};

struct Proghdr {
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_va;
	uint32_t p_pa;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
};

struct Secthdr {
	uint32_t sh_name;
	uint32_t sh_type;
	uint32_t sh_flags;
	uint32_t sh_addr;
	uint32_t sh_offset;
	uint32_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint32_t sh_addralign;
	uint32_t sh_entsize;
};

// Values for Proghdr::p_type
#define PT_NULL		0
#define PT_LOAD		1

// Flag bits for Proghdr::p_flags
#define PF_X		1
#define PF_W		2
#define PF_R		4

// Values for Secthdr::sh_type
#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3

// Values for Secthdr::sh_name
#define SHN_UNDEF	0
