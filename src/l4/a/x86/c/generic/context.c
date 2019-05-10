#include <l4/generic/context.h>
#include <l4/generic/task-switch.h>
#include <l4/system/kstack.h>
#include <l4/system/asm/gdt.h>
#include <l4/machine/asm/eflags.h>
#include <l4/misc/bug.h>
#include <l4/misc/printk.h>//
#include <memory.h>

#define DEFAULT_EFLAGS (FL_1F | FL_IF | FL_IOPL(3))

/*void task_newly_forked(struct ktcb *task)
{
	//printk("task_newly_forked");
	//save_context(&task->context);
	task->context.eax = 0;
	task->context.sp = kSEFrame.sp;
	task->context.pc = kSEFrame.pc;
	task->context.eflags = DEFAULT_EFLAGS;
}*/

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
	kIFrame.ds = SEG_UDATA;
	kIFrame.es = SEG_UDATA;
	kIFrame.fs = SEG_UDATA;
	kIFrame.gs = SEG_UDATA;

	//printk("rstr pc=%p", ctx->pc);

	extern void _NORETURN utcb_exiter(void);
	kErnelExiter = utcb_exiter;
}

void context_init(struct context *ctx)
{
	memset(ctx, 0, sizeof(*ctx));
	ctx->eflags = DEFAULT_EFLAGS;
}

void save_context(struct context *ctx)
{
	extern void _NORETURN utcb_exiter(void);
	extern void _NORETURN seframe_exiter(void);

	//printk("%p", kErnelExiter);
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

	} else if (/**1||**/kErnelExiter == seframe_exiter) {
		//BUG();
		ctx->eax = kSEFrame.eax;
		ctx->sp = kSEFrame.sp;
		ctx->pc = kSEFrame.pc;
		//printk("save pc=%p", ctx->pc);
		ctx->ecx = 0;
		ctx->edx = 0;
		ctx->ebx = 0;
		ctx->esi = 0;
		ctx->edi = 0;
		ctx->ebp = 0;
		ctx->eflags = DEFAULT_EFLAGS;

	} else {
		BUG();
	}
}
