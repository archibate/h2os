#include <k/kstack.h>
#include <l4/invoke.h>
#include <l4/syscall.h>

void hwsysintr(void)
{
	Invo_t invo;
	invo.service = kIFrame[IFrame_EAX];
	cptr_t cptr = kIFrame[IFrame_EBX];
	invo.offset = kIFrame[IFrame_ESI];
	invo.length = kIFrame[IFrame_EDI];
	invo.dataSend = (void*)kIFrame[IFrame_EBP];

	kIFrame[IFrame_EAX] = systemCall(cptr, &invo);
}
