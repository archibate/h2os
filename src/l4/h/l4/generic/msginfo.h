#pragma once

#include <l4/object/msginfo.h>

//void msginfo_copy(struct msginfo *dst, const struct msginfo *src);
#define msginfo_copy(dst, src) (*(dst) = *(src))
