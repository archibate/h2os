#include <l4/a/tswitch.h>
#include <l4/a/pgdir.h>
#include <k/asm/seframe.h>
#include <k/asm/iframe.h>
#include <l4/asm/context.h>
#include <l4/asm/shortmsg.h>
#include <x86/asmregs.h>
#include <k/kstack.h>
#include <stassert.h>
#include <assert.h>

void Arch_switchTask(tcb_t *oldTcb, tcb_t *newTcb)
{
	Arch_switchPgdirAndUTCB(
			//(void*)oldTcb->t_pgdirAddr,
			(void*)newTcb->t_pgdirAddr,
			newTcb->t_utcbAddr);

	extern void utcb_iframe_exiter(void);
	kSEFrame[-1] = (word_t)utcb_iframe_exiter;
}
#if 0 // {{{
static bool is_seframe(void)
{
	static_assert(KernIStackTop < KernSEStackTop);
	return get_reg(esp) > KernIStackTop;
}

void switch_context(word_t oldCtx[L4_ContextWords],
		    word_t newCtx[L4_ContextWords])
{
	extern void iframe_exiter(void);
	extern void seframe_exiter(void);
	extern word_t seShortMsg[L4_ShortMsgWords];

	if (is_seframe()) {
		oldCtx[L4_Context_EIP] = kSEFrame[SEFrame_ESP];
		oldCtx[L4_Context_ESP] = kSEFrame[SEFrame_ESP];
		oldCtx[L4_Context_EAX] = 0;
		oldCtx[L4_Context_ECX] = 0;
		oldCtx[L4_Context_EDX] = 0;
		oldCtx[L4_Context_EBX] = seShortMsg[L4_ShortMsg_EBX];
		oldCtx[L4_Context_EDI] = seShortMsg[L4_ShortMsg_EDI];
		oldCtx[L4_Context_ESI] = seShortMsg[L4_ShortMsg_ESI];
		oldCtx[L4_Context_EBP] = seShortMsg[L4_ShortMsg_EBP];
	} else {
		oldCtx[L4_Context_EIP] = kIFrame[IFrame_EIP];
		oldCtx[L4_Context_ESP] = kIFrame[IFrame_ESP];
		oldCtx[L4_Context_EAX] = kIFrame[IFrame_EAX];
		oldCtx[L4_Context_ECX] = kIFrame[IFrame_ECX];
		oldCtx[L4_Context_EDX] = kIFrame[IFrame_EDX];
		oldCtx[L4_Context_EBX] = kIFrame[IFrame_EBX];
		oldCtx[L4_Context_EDI] = kIFrame[IFrame_EDI];
		oldCtx[L4_Context_ESI] = kIFrame[IFrame_ESI];
		oldCtx[L4_Context_EBP] = kIFrame[IFrame_EBP];
	}

	kIFrame[IFrame_EIP] = newCtx[L4_Context_EIP];
	kIFrame[IFrame_ESP] = newCtx[L4_Context_ESP];
	kIFrame[IFrame_EAX] = newCtx[L4_Context_EAX];
	kIFrame[IFrame_ECX] = newCtx[L4_Context_ECX];
	kIFrame[IFrame_EDX] = newCtx[L4_Context_EDX];
	kIFrame[IFrame_EBX] = newCtx[L4_Context_EBX];
	kIFrame[IFrame_EDI] = newCtx[L4_Context_EDI];
	kIFrame[IFrame_ESI] = newCtx[L4_Context_ESI];
	kIFrame[IFrame_EBP] = newCtx[L4_Context_EBP];
	assert(kSEFrame[-1] = (word_t)seframe_exiter);
	kSEFrame[-1] = (word_t)iframe_exiter;
}
#endif // }}}
