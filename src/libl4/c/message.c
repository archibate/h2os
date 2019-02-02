#include <libl4/message.h>
#include <libl4/syscall.h>
#include <l4/asm/shortmsg.h>
#include <numtools.h>
#include <memory.h>

extern int l4ExtraRewind(void);
extern int l4ExtraWrite(const void *buf, size_t len);

static size_t msg_bytes;
#define L4_ShortMsgBytes  (L4_ShortMsgWords * sizeof(l4Word_t))
static l4Byte_t msg_shrt[L4_ShortMsgBytes];

void l4MsgInit(void)
{
	msg_bytes = 0;
	memset(msg_shrt, 0, sizeof(msg_shrt));
	l4ExtraRewind();
}

l4Ret_t l4MsgInvoke(l4CPtr_t cptr)
{
	return l4Syscall(cptr, (l4Word_t*)&msg_shrt);
}

size_t l4MsgWrite(const void *buf, size_t size)
{
	if (msg_bytes < L4_ShortMsgBytes) {
		size_t len = MIN(size, L4_ShortMsgBytes - msg_bytes);
		memcpy(msg_shrt + msg_bytes, buf, len);
		msg_bytes += len;
		buf += len;
		size -= len;
		if (!size)
			return 0;
	}
	ssize_t ret = l4ExtraWrite(buf, size);
	if (ret >= 0) {
		msg_bytes += size - ret;
		size = ret;
	}
	return size;
}

#include <libl4/buffer.h>
#include <libl4/captrs.h>

static int need_rewind = 1;

int l4ExtraRewind(void)
{
	need_rewind = 1;
	return 0;
}

int l4ExtraWrite(const void *buf, size_t len)
{
	l4Buffer_WriteFrags(Libl4_CapExtra, buf, len, need_rewind);
	need_rewind = 0;
	return 0;
}
