#include <libl4/mm.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

/**
 * split a segment in two at a specific point
 *
 * @param cptr	capability to the segment
 *
 * @param point	at which segment is cut
 *
 * @return	the kernel return value
 *
 * @retval 0		success
 *
 * @retval -L4_EWater	specific `point` not above water
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
 * alloc page(s) from a segment
 *
 * @param cptr	capability to the segment
 *
 * @param count	how many pages to allocate
 *
 * @return	the kernel return value
 *
 * @retval 0	success
 *
 * @retval >0	the number of pages NOT allocated
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Segment_AllocPage(l4CPtr_t cptr, l4Word_t count)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Segment_AllocPage,
		[L4_Segment_AllocPage_Arg_Count] = count,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}
