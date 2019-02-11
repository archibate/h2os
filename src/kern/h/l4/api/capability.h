#pragma once

#include <l4/lib/list.h>

struct capability
{
	struct hlist_node hlist;
	unsigned short ctype;
	unsigned short rtype;
	unsigned int access;

	l4id_t resid;
	unsigned long base;
	unsigned long size;
	unsigned long used;
};
