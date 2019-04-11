#pragma once

#include <l4/object/mregion.h>

bool mreg_inside(struct mregion *mreg, word_t start, word_t end);
bool mreg_hasisect(struct mregion *mreg, word_t start, word_t end);
