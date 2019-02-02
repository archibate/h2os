#include <libl4/invoke.h>
#include <libl4/message.h>
#include <libl4/errors.h>

int l4Invoke(l4CPtr_t cptr, const void *buf, size_t size)
{
	l4MsgInit();
	if (l4MsgWrite(buf, size))
		return -Libl4_Error;
	return l4MsgInvoke(cptr);
}
#if 0 // {{{
ssize_t l4Invokev(l4CPtr_t cptr, const struct iovec *iov, int iovcnt)
{
	size_t size = 0;
	ssize_t ret;
	l4Message_t msg;
	l4MsgInit(&msg);
	int i;
	for (i = 0; i < iovcnt; i++) {
		ssize_t ret = l4MsgWrite(&msg, iov[i].iov_base, iov[i].iov_len);
		if (ret < 0)
			return ret;
		size += iov[i].iov_len - ret;
		if (ret)
			break;
	}
	return l4MsgInvoke(&msg, cptr);
}
#endif // }}}
