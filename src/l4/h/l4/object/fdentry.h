#pragma once

struct fd_entry
{
	void *ptr;
	unsigned int rtype;
	unsigned int flags;
	struct msginfo msginfo;
};
