#include <l4/a/utcb.h>
#include <k/x86/gdt.h>
#include <x86/eflags.h>

void UTCB_Init(utcb_t *utcb, word_t pc, word_t sp)
{
	word_t cs = SEG_UCODE;
	word_t ds = SEG_UDATA;
	word_t eflags = FL_1F | FL_IF | FL_IOPL(3);
	utcb->seframe[SEFrame_DS] = ds;
	utcb->seframe[SEFrame_ES] = ds;
	utcb->iframe[IFrame_CS] = cs;
	utcb->iframe[IFrame_SS] = ds;
	utcb->iframe[IFrame_DS] = ds;
	utcb->iframe[IFrame_ES] = ds;
	utcb->iframe[IFrame_FS] = ds;
	utcb->iframe[IFrame_GS] = ds;
	utcb->iframe[IFrame_EFLAGS] = eflags;
	utcb->iframe[IFrame_PC] = pc;
	utcb->iframe[IFrame_SP] = sp;
	utcb->seframe[SEFrame_PC] = pc;
	utcb->seframe[SEFrame_SP] = sp;
}//T:user.c mo use Arch_InitUTCB!
