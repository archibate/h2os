#include <k/kstack.h>
#include <l4/syscall.h>
#include <l4/asm/shortmsg.h>

void hwsysintr(void)
{
	word_t shrt[L4_ShortMsgWords];
	cptr_t cptr = kIFrame[IFrame_EAX];
	shrt[L4_ShortMsg_EBX] = kIFrame[IFrame_EBX];
	shrt[L4_ShortMsg_EDI] = kIFrame[IFrame_EDI];
	shrt[L4_ShortMsg_ESI] = kIFrame[IFrame_ESI];
	shrt[L4_ShortMsg_EBP] = kIFrame[IFrame_EBP];
	kIFrame[IFrame_EAX] = systemCall(cptr, shrt);
}
