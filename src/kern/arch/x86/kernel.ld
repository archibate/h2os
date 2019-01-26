#include <k/kbase.h>
#define PGSIZE 4096

#ifndef _MINGW
ENTRY(_start)
#else
ENTRY(__start)
#endif
#ifndef _MINGW
OUTPUT_FORMAT("elf32-i386")
#endif

/*#ifndef _MINGW
#define PSEC(sec) sec :
#define VSEC(sec) sec : AT(ADDR(sec) - KBASE)
#else
#define PSEC(sec) sec :
#define VSEC(sec) sec :
#endif*/

/* https://github.com/archibate/jos/blob/master/lab4/partB/kern/kernel.ld */
SECTIONS
{
	. = KPBEG;

	.header :
	{
		*(.mboot_header)
		*(.stub)
	}

	/*. += KBASE;*/

	.text :
	{
		*(.text .text.* .gnu.linkonce.t.*)
	}
	.rodata :
	{
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}
	.stab :
	{
		*(.stab)
	}
	.stabstr :
	{
		*(.stabstr)
	}

	. = ALIGN(PGSIZE);
	.data :
	{
		*(.data .data.*)
		*(.note* .comment*)
	}

	. = ALIGN(PGSIZE);
	.bss :
	{
		*(.bss .bss.*)
	}

	/DISCARD/ :
	{
		*(.eh_frame .note.GNU-stack)
	}
}
