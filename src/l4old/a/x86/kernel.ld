#include <l4/system/kbase.h>
#include <l4/machine/mmu/page.h>

ENTRY(_start)
#ifndef _MINGW
OUTPUT_FORMAT("elf32-i386")
#endif

/* https://github.com/archibate/jos/blob/master/lab4/partB/kern/kernel.ld */
SECTIONS
{
	. = KernPhysBegin;

	.header :
	{
		*(.mboot_header)
		*(.stub)
	}

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

	. = ALIGN(PageSize);
	.data :
	{
		*(.data .data.*)
		*(.note* .comment*)
	}

	. = ALIGN(PageSize);
	.bss :
	{
		*(.bss .bss.*)
		*(COMMON)
	}

	/DISCARD/ :
	{
		*(.eh_frame .note.GNU-stack)
	}
}
