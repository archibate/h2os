#pragma once

#include <l4/object/msginfo.h>

struct fd_entry
{
	void *ptr;
	unsigned int rtype;
	struct msginfo msginfo;
};
