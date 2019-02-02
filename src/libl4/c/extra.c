#include <libl4/extra.h>
#include <libl4/syscall.h>
#include <libl4/captrs.h>
#include <l4/arguments.h>
#include <l4/services.h>
#include <numtools.h>
#include <memory.h>

#define L4_ExtraFragSize (sizeof(l4Word_t) * (L4_ShortMsgWords - L4_RWFragArg_DataBegin))

static int need_rewind = 1;

int l4ExtraRewind(void)
{
	need_rewind = 1;
	return 0;
}

int l4ExtraWrite(const void *buf, size_t size)
{
	ssize_t ret;
	size_t len;
	l4Word_t shrt[L4_ShortMsgWords];
	do {
		len = MIN(size, L4_ExtraFragSize);
		//asm volatile ("jmp ." :: "a" (len), "b" (size));
		// len=0xc, size=0x1d
		memcpy(shrt + L4_RWFragArg_DataBegin, buf, len);
		shrt[L4_Arg_Service] = need_rewind ? L4_RewindWriteFrag : L4_WriteFrag;
		ret = l4Syscall(Libl4_CapExtra, shrt);
		need_rewind = 0;
		if (ret)
			break;
		buf += len;
		size -= len;
		//asm volatile ("jmp ." :: "a" (len), "b" (size));
	} while (size > 0);
	return size;
}
