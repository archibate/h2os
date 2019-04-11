#pragma once

#include <l4/object/mregion.h>

struct mm
{
	struct hlist_head mregs;
};
