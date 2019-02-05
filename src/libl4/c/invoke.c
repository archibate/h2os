#include <libl4/invoke.h>
#include <libl4/message.h>
#include <libl4/errors.h>

/**
 * invoke a capability with some message
 *
 * @param cptr	capability to invoke
 *
 * @param buf	data to send
 *
 * @param size	size of the data
 *
 * @return	the kernel return value
 *
 * @retval -Libl4_Error	unexcepted error
 */
l4Ret_t l4Invoke(l4CPtr_t cptr, const void *buf, size_t size)
{
	l4MsgInit();
	if (l4MsgWrite(buf, size))
		return -Libl4_Error;
	return l4MsgInvoke(cptr);
}

#if 0
/**
 * invoke a capability with some message, and some caps
 *
 * @param cptr	capability to invoke
 *
 * @param buf	data to send
 *
 * @param size	size of the data
 *
 * @param caps	capability pointers to send
 *
 * @param capnr	number of capabilities to send
 *
 * @return	the kernel return value
 *
 * @retval -Libl4_Error	unexcepted error
 */
l4Ret_t l4CapInvoke(l4CPtr_t cptr, const void *buf, size_t size,
		    const l4CPtr_t *caps, size_t capnr)
{
	l4MsgInit();
	if (l4MsgWrite(buf, size))
		return -Libl4_Error;
	if (l4MsgWrite(caps, capnr))
		return -Libl4_Error;
	return l4MsgInvoke(cptr);
}
#endif
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
