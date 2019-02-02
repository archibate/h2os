#include <libl4/io.h>
#include <libl4/errors.h>
#include <libl4/message.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>
#include <numtools.h>

/**
 * @function	l4PWrite
 *
 * @brief	write data into a capability at a specific offset
 *
 * @param cptr	capability pointer
 *
 * @param buf	data to write
 *
 * @param size	size of the data
 *
 * @param off	offset within the target
 *
 * @retval >=0	size remain not written
 *
 * @retval <0	error from kernel
 *
 * @retval -Libl4_Error	unexcepted error
 */
ssize_t l4PWrite(l4CPtr_t cptr, const void *buf, size_t size, off_t off)
{
	size_t bufsiz = l4MsgGetBufSize();
	l4Word_t header[] =
	{
		[L4_Arg_Service] = L4_PWrite,
		[L4_PRWArg_Length] = 0,
		[L4_PRWArg_Offset] = off,
	};
	size_t size_write = MIN(size, bufsiz - sizeof(header));
	header[L4_PRWArg_Length] = size_write;
	l4MsgInit();
	if (l4MsgWrite(header, sizeof(header)))
		return -Libl4_Error;
	if (l4MsgWrite(buf, size_write))
		return -Libl4_Error;
	return l4MsgInvoke(cptr);
}
