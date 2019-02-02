#include <libl4/io.h>
#include <libl4/message.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

ssize_t l4Write(l4CPtr_t cptr, const void *data, l4Word_t size)
{
	ssize_t ret;
	l4Word_t header[] =
	{
		[L4_Arg_Service] = L4_Write,
		[L4_RWArg_Length] = size,
	};
	l4MsgInit();
	ret = l4MsgWrite(header, sizeof(header));
	if (ret)
		return ret < 0 ? ret : size;
	ret = l4MsgWrite(data, size);
	if (ret < 0)
		return ret;
	else
		size -= ret;
	ret = l4MsgInvoke(cptr);
	return ret; // ret < 0 ? ret : MIN(ret, size);
}
