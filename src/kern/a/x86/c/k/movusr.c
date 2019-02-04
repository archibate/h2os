#include <k/movusr.h>
#include <k/x86/gdt.h>
#include <k/asm/iframe.h>
#include <memory.h>

void _NORETURN goto_user_entry(void *pc, void *sp)
{
	ulong uc[IFrameWords];
	memset(uc, 0, sizeof(uc));
	uc[IFrame_PC] = (ulong)pc;
	uc[IFrame_SP] = (ulong)sp;
	uc[IFrame_EFLAGS] = 0x202;
	uc[IFrame_CS] = SEG_UCODE;
	uc[IFrame_SS] = SEG_UDATA;
	uc[IFrame_DS] = SEG_UDATA;
	uc[IFrame_ES] = SEG_UDATA;
	uc[IFrame_FS] = SEG_UDATA;
	uc[IFrame_GS] = SEG_UDATA;
	move_to_user(uc); // intrents.asm
}
