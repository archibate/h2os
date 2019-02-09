#pragma once

#include <l4/capability.h>

int do_Segment_Split(cap_t *segm, cap_t *capDest, word_t point);
int do_Segment_AllocPage(cap_t *segm, cap_t *capDest, word_t num);
