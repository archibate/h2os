#pragma once

struct fd_entry
{
	void *ptr;
	unsigned int rtype;
	struct msginfo msginfo;
};
