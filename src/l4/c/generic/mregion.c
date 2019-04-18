#include <l4/generic/mregion.h>

bool mreg_inside(struct mregion *mreg, word_t start, word_t end)
{
	return mreg->start <= start && mreg->end >= end;
}

bool mreg_hasisect(struct mregion *mreg, word_t start, word_t end)
{
	return start >= end && (mreg->start <= start || mreg->end >= end);
}
