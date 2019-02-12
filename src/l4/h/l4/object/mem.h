#pragma once

#include <l4/object/identry.h>

struct mem
{
	struct ids_entry ide;

	unsigned long start;
	unsigned long end;
	unsigned long used;
};
