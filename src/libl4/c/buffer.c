#include <libl4/buffer.h>
#include <libl4/syscall.h>
#include <l4/arguments.h>
#include <l4/services.h>
#include <numtools.h>
#include <memory.h>

#define L4_BufferFragSize (sizeof(l4Word_t) * (L4_ShortMsgWords - L4_RWFragArg_DataBegin))

l4Ret_t l4Buffer_WriteFrags(l4CPtr_t cptr, const void *buf, size_t size, int rewind)
{
	l4Ret_t ret;
	size_t len;
	l4Word_t shrt[L4_ShortMsgWords];
	do {
		len = MIN(size, L4_BufferFragSize);
		memcpy(shrt + L4_RWFragArg_DataBegin, buf, len);
		shrt[L4_Arg_Service] = rewind ? L4_RewindWriteFrag : L4_WriteFrag;
		ret = l4Syscall(cptr, shrt);
		rewind = 0;
		if (ret)
			return ret;
		buf += len;
		size -= len;
	} while (size > 0);
	return 0;
}

l4Ret_t l4Buffer_ReadFrags(l4CPtr_t cptr, void *buf, size_t size, int rewind)
{
	l4Ret_t ret;
	size_t len;
	l4Word_t shrt[L4_ShortMsgWords];
	do {
		len = MIN(size, L4_BufferFragSize);
		memcpy(buf, shrt + L4_RWFragArg_DataBegin, len);
		shrt[L4_Arg_Service] = rewind ? L4_RewindReadFrag : L4_ReadFrag;
		ret = l4Syscall(cptr, shrt);
		rewind = 0;
		if (ret)
			return ret;
		buf += len;
		size -= len;
	} while (size > 0);
	return 0;
}
