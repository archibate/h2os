#include <libl4/mm.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

/**
 * retype a page into a slab
 *
 * @param cptr	capability to the page
 *
 * @param type	on which type is slab retyped
 *
 * @return	the kernel return value
 *
 * @retval 0	success
 *
 * @retval -L4_ERetype	given `type` is not a valid slab type
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Page_RetypeToSlab(l4CPtr_t cptr, l4Byte_t type)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Page_RetypeToSlab,
		[L4_Page_RetypeToSlab_Arg_ObjType] = type,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

/**
 * allocate object(s) from a slab
 *
 * @param cptr	capability to the slab
 *
 * @param count	how many objects to create
 *
 * @return	the kernel return value
 *
 * @retval 0	success
 *
 * @retval >0	the number of objects NOT created
 *
 * @retval -L4_ERetype	slab not retyped
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Slab_Allocate(l4CPtr_t cptr, l4Word_t count)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Slab_Allocate,
		[L4_Slab_Allocate_Arg_Count] = count,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}
