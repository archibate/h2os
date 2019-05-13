#pragma once

#include <l4/object/msginfo.h>
#include <l4/object/identry.h>

struct fd_entry
{
	struct id_entry *ide;
	struct endpoint *ep;
	struct msginfo msginfo;
};
