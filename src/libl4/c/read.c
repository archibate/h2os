#include <libl4/io.h>
#include <libl4/errors.h>
#include <libl4/message.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>
#include <numtools.h>

/**
 * @function	l4Read
 *
 * @brief	read data from a capability
 *
 * @param cptr	capability pointer
 *
 * @param buf	buffer for read
 *
 * @param size	size of the buffer
 *
 * @retval >=0	size remain not valid in buffer
 *
 * @retval <0	error from kernel
 *
 * @retval -Libl4_Error	unexcepted error
 */
ssize_t l4Read(l4CPtr_t cptr, void *buf, size_t size)
{
	size_t bufsiz = l4MsgGetBufSize();
	l4Word_t header[] =
	{
		[L4_Arg_Service] = L4_Read,
		[L4_RWArg_Length] = 0,
	};
	size_t size_read = MIN(size, bufsiz - sizeof(header));
	header[L4_RWArg_Length] = size_read;
	l4MsgInit();
	if (l4MsgWrite(header, sizeof(header)))
		return -Libl4_Error;
	ssize_t ret = l4MsgInvoke(cptr);
	if (ret < 0)
		return ret;
	else
		size_read -= ret;
	if (l4MsgRead(buf, size_read))
		return -Libl4_Error;
	return size - size_read;
}
