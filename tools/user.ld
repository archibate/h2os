#include <l4/system/kbase.h>
#include <l4/machine/mmu/page.h>

ENTRY(_start)
#ifndef _MINGW
OUTPUT_FORMAT("elf32-i386")
#else
#define PROVIDE(x) PROVIDE(_##x)
#endif

/* https://github.com/archibate/jos/blob/master/lab4/partB/kern/kernel.ld */
SECTIONS
{
	/*. = KernVirtEnd;*/
	. = 0x10000000;

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
		PROVIDE(__STAB_BEGIN__ = .);
		*(.stab)
		PROVIDE(__STAB_END__ = .);
	}
	.stabstr :
	{
		PROVIDE(__STABSTR_BEGIN__ = .);
		*(.stabstr)
		PROVIDE(__STABSTR_END__ = .);
	}

	.rodata_align :
	{
		. = ALIGN(PageSize);
	}

	.data :
	{
		*(.data .data.*)
		*(.note* .comment*)
	}

	.ctors :
	{
		PROVIDE(__CTORS_BEGIN__ = .);
		*(.ctors .ctors.*)
		PROVIDE(__CTORS_END__ = .);
		LONG(0);
	}

	.dtors :
	{
		PROVIDE(__DTORS_BEGIN__ = .);
		*(.dtors .dtors.*)
		PROVIDE(__DTORS_END__ = .);
		LONG(0);
	}

	.data_align :
	{
		. = ALIGN(PageSize);
	}

	.bss :
	{
		*(.bss .bss.*)
		*(COMMON)
	}

	.bss_align :
	{
		. = ALIGN(PageSize);
		PROVIDE(__EBSS__ = .);
	}

	/DISCARD/ :
	{
		*(.eh_frame .note.GNU-stack)
	}
}
