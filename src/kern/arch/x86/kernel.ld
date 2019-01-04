#include <hydro/kbase.h>
#include <asm/page.h>

ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")

#define PSEC(sec) sec :
#define VSEC(sec) sec : AT(ADDR(sec) - KBASE)

/* https://github.com/archibate/jos/blob/master/lab4/partB/kern/kernel.ld */
SECTIONS
{
	. = KPBEG;

	PSEC(.phys)
	{
		*(.phys.header)
		*(.phys.stub)
		*(.phys .phys.*)
	}

	. += KBASE;

	VSEC(.text)
	{
		*(.text .text.* .gnu.linkonce.t.*)
	}
	VSEC(.rodata)
	{
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}
	VSEC(.stab)
	{
		*(.stab)
	}
	VSEC(.stabstr)
	{
		*(.stabstr)
	}

	. = ALIGN(PGSIZE);
	VSEC(.data)
	{
		*(.data .data.*)
		*(.note* .comment*)
	}

	. = ALIGN(PGSIZE);
	VSEC(.bss)
	{
		*(.bss .bss.*)
	}

	/DISCARD/ :
	{
		*(.eh_frame .note.GNU-stack)
	}
}
