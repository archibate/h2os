#pragma once

#include <l4/lib/list.h>

struct capability
{
	struct hlist_node hlist;

	unsigned short rtype;
	unsigned char ctype;

	unsigned char access;

	void *pointer;

	unsigned long base;
	unsigned long size;
	unsigned long used;
};
