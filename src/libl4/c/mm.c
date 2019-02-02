#include <libl4/mm.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

int l4Segment_Split(l4CPtr_t cptr, l4Byte_t point)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Segment_Split,
		[L4_Segment_Split_Arg_Point] = point,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

int l4Segment_AllocSlab(l4CPtr_t cptr, l4Word_t count)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Segment_AllocSlab,
		[L4_Segment_AllocSlab_Arg_Count] = count,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

int l4Slab_Retype(l4CPtr_t cptr, l4Byte_t type)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Slab_Retype,
		[L4_Slab_Retype_Arg_ObjType] = type,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

int l4Slab_Allocate(l4CPtr_t cptr, l4Word_t count)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Slab_Allocate,
		[L4_Slab_Allocate_Arg_Count] = count,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}
