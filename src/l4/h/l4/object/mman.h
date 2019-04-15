#pragma once

#include <l4/object/mregion.h>

struct mm
{
	struct hlist_head mregs;
	char buf[245];
	struct pgdir *pgdir;
};
