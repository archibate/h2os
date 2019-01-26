#include <k/kbase.h>
#include <asm/mmu/page.h>

#ifndef _MINGW
ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")
#else
ENTRY(__start)
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
	}

	/DISCARD/ :
	{
		*(.eh_frame .note.GNU-stack)
	}
}
