#pragma once

#include <l4/types.h>

#define _L4_DO_SYSCALL(res, cptr, service, offset, data, length) \
	asm volatile ( \
		"push %%ebp\n" \
		"mov %%edx, %%ebp\n" \
		"call libl4_do_syscall\n" \
		"pop %%ebp\n" \
		: "=a" (res) \
		: "b" (cptr) \
		, "a" (service) \
		, "S" (offset) \
		, "D" (length) \
		, "d" (data) \
		: "cc", "memory")

static int l4Send(l4CPtr_t cptr, l4Word_t service, l4Word_t offset, const void *data, l4Word_t length)
{
	int res;
	_L4_DO_SYSCALL(res, cptr, service, offset, data, length);
	return res;
}

static int l4Recv(l4CPtr_t cptr, l4Word_t service, l4Word_t offset, void *data, l4Word_t length)
{
	int res;
	_L4_DO_SYSCALL(res, cptr, service, offset, data, length);
	return res;
}
