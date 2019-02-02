#include <libl4/buffer.h>
#include <libl4/syscall.h>
#include <l4/arguments.h>
#include <l4/services.h>
#include <numtools.h>
#include <memory.h>

#define L4_BufferFragSize (sizeof(l4Word_t) * (L4_ShortMsgWords - L4_RWFragArg_DataBegin))

int l4Buffer_WriteFrags(l4CPtr_t cptr, const void *buf, size_t size, int rewind)
{
	ssize_t ret;
	size_t len;
	l4Word_t shrt[L4_ShortMsgWords];
	do {
		len = MIN(size, L4_BufferFragSize);
		//asm volatile ("jmp ." :: "a" (len), "b" (size));
		// len=0xc, size=0x1d
		memcpy(shrt + L4_RWFragArg_DataBegin, buf, len);
		shrt[L4_Arg_Service] = rewind ? L4_RewindWriteFrag : L4_WriteFrag;
		ret = l4Syscall(cptr, shrt);
		rewind = 0;
		if (ret)
			break;
		buf += len;
		size -= len;
		//asm volatile ("jmp ." :: "a" (len), "b" (size));
	} while (size > 0);
	return size;
}
