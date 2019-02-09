#pragma once

#include <l4/capability.h>

#if 0
int do_Segment_Split(CSegment_t *segm, cap_t *capDest, word_t point);
#endif
int do_Segment_AllocPage(CSegment_t *segm, cap_t *capDest, word_t num);
