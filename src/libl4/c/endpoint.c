#include <libl4/endpoint.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

/**
 * @function	l4Endpoint_Retype
 *
 * @brief	retype an endpoint
 *
 * @param cptr	capability to the endpoint
 *
 * @param type	new endpoint type
 *
 * @return	the kernel return value
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Endpoint_Retype(l4CPtr_t cptr, l4Byte_t type)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Endpoint_Retype,
		[L4_Endpoint_Retype_Arg_EPType] = type,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}
