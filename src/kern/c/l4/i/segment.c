#include <l4/i/segment.h>
#include <l4/captypes.h>
#include <l4/errors.h>
#include <mmu/page.h>
#include <memory.h>
#include <assert.h>

int do_Segment_Split(cap_t *segm, cap_t *capDest, word_t point)
{
	if (point <= segm->c_water)
		return -L4_EWater;
	// T: verify(dest)
	cap_t *dest = capDest;
	segm->c_limit = point;
	memset(dest, 0, sizeof(cap_t));
	dest->c_base = segm->c_base + segm->c_limit;
	dest->c_limit = segm->c_limit - point;
	return 0;
}

int do_Segment_AllocPage(cap_t *segm, cap_t *capDest, word_t num)
{
	cap_t *dest = capDest;
	//assert(segm != dest);
	word_t water = PageUp(segm->c_base + segm->c_water);
	word_t end = PageDown(segm->c_base + segm->c_limit);
	if (end < water)
		return num;
	while (num > 0) {
		if (water >= end)
			break;
		//dprintk("L4_Segment_AllocPage: water = %p", water);
		memset(dest, 0, sizeof(cap_t));
		dest->c_type = L4_PageCap;
		dest->c_objaddr = water;
		water += PageSize;
		dest++;
		num--;
	}
	assert(!num);
	segm->c_water = water - segm->c_base;
	return num;
}
