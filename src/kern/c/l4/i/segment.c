#include <l4/i/segment.h>
#include <l4/captypes.h>
#include <l4/errors.h>
#include <mmu/page.h>
#include <memory.h>
#include <assert.h>

#if 0
int do_Segment_Split(CSegment_t *segm, cap_t *capDest, word_t point)
{
	if (point <= segm->water)
		return -L4_EWater;
	if (point > segm->limit)
		return -L4_ELimit;
	// T: verify(dest)
	cap_t *dest = capDest;
	segm->limit = point;
	memset(dest, 0, sizeof(cap_t));
	dest->ctype = L4_SegmentCap;
	dest->c_segment.base = segm->base + segm->limit;
	dest->c_segment.limit = segm->limit - point;
	return 0;
}
#endif

int do_Segment_AllocPage(CSegment_t *segm, cap_t *capDest, word_t num)
{
	cap_t *dest = capDest;
	//assert(segm != dest);
	word_t water = segm->base + segm->water;
	word_t end = segm->base + segm->limit;
	if (end < water)
		return num;
	while (num > 0) {
		if (water >= end)
			break;
		//dprintk("L4_Segment_AllocPage: water = %p", water);
		memset(dest, 0, sizeof(cap_t));
		dest->ctype = L4_PageCap;
		dest->c_objaddr = (water << PageBits);
		cdepend(dest, segm);
		water++;
		dest++;
		num--;
	}
	assert(!num);
	segm->water = water - segm->base;
	return num;
}
