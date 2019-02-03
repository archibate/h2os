#include <libl4/mm.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

/**
 * split a segment into ahalf
 *
 * @param cptr	capability to the segment
 *
 * @param point	at which segment is cut
 *
 * @return	the kernel return value
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Segment_Split(l4CPtr_t cptr, l4Byte_t point)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Segment_Split,
		[L4_Segment_Split_Arg_Point] = point,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

/**
 * alloc slab(s) from a segment
 *
 * @param cptr	capability to the segment
 *
 * @param count	how many slabs to create
 *
 * @return	the kernel return value
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Segment_AllocSlab(l4CPtr_t cptr, l4Word_t count)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Segment_AllocSlab,
		[L4_Segment_AllocSlab_Arg_Count] = count,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}
