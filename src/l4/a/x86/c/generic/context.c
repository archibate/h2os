#include <l4/generic/context.h>
#include <l4/system/kstack.h>
#include <l4/system/asm/gdt.h>
#include <l4/machine/asm/eflags.h>
#include <l4/misc/bug.h>
#include <memory.h>

void restore_context(const struct context *ctx)
{
	kIFrame.eax = ctx->eax;
	kIFrame.ecx = ctx->ecx;
	kIFrame.edx = ctx->edx;
	kIFrame.ebx = ctx->ebx;
	kIFrame.esi = ctx->esi;
	kIFrame.edi = ctx->edi;
	kIFrame.ebp = ctx->ebp;
	kIFrame.pc = ctx->pc;
	kIFrame.sp = ctx->sp;
	kIFrame.eflags = ctx->eflags;
	kIFrame.cs = SEG_UCODE;
	kIFrame.ss = SEG_UDATA;

	extern void _NORETURN utcb_exiter(void);
	kErnelExiter = utcb_exiter;
}

void context_init(struct context *ctx)
{
	memset(ctx, 0, sizeof(*ctx));
	ctx->eflags = FL_1F | FL_IF | FL_IOPL(3);
}

void save_context(struct context *ctx)
{
	extern void _NORETURN utcb_exiter(void);
	extern void _NORETURN seframe_exiter(void);

	if (kErnelExiter == utcb_exiter) {
		BUG_ON(~kIFrame.cs & 3);
		ctx->eax = kIFrame.eax;
		ctx->ecx = kIFrame.ecx;
		ctx->edx = kIFrame.edx;
		ctx->ebx = kIFrame.ebx;
		ctx->esi = kIFrame.esi;
		ctx->edi = kIFrame.edi;
		ctx->ebp = kIFrame.ebp;
		ctx->pc = kIFrame.pc;
		ctx->sp = kIFrame.sp;
		ctx->eflags = kIFrame.eflags;

	} else if (kErnelExiter == seframe_exiter) {
		ctx->eax = kSEFrame.eax;
		ctx->sp = kSEFrame.sp;
		ctx->pc = kSEFrame.pc;

	} else {
		BUG();
	}
}
