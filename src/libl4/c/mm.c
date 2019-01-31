#include <libl4/mm.h>
#include <libl4/api.h>
#include <l4/services.h>
#include <stddef.h>

int l4Segment_Split(l4CPtr_t cptr, l4Byte_t point)
{
	return l4Send(cptr, L4_Segment_Split, point, 0, 0);
}

int l4Segment_AllocSlab(l4CPtr_t cptr, l4Word_t num)
{
	return l4Send(cptr, L4_Segment_AllocSlab, 0, 0, 0); // T: capCount=num (extraCaps)
}

int l4Slab_Retype(l4CPtr_t cptr, l4Byte_t retype)
{
	return l4Send(cptr, L4_Slab_Retype, retype, 0, 0);
}

int l4Slab_Allocate(l4CPtr_t cptr, l4Word_t num)
{
	return l4Send(cptr, L4_Slab_Allocate, 0, 0, 0); // T: capCount=num (extraCaps)
}
