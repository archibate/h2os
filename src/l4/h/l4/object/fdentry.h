#pragma once

#include <l4/object/msginfo.h>

struct fd_entry
{
	struct endpoint *ep;
	struct msginfo msginfo;
};
