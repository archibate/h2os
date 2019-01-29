#pragma once

#include <libl4/invoke.h>
#include <l4/services.h>

static int l4Read(l4CPtr_t cptr, l4Word_t offset, void *data, l4Word_t length)
{
	return l4Invoke(cptr, L4_Read, offset, data, length);
}

static int l4Write(l4CPtr_t cptr, l4Word_t offset, const void *data, l4Word_t length)
{
	return l4Invoke(cptr, L4_Write, offset, (void*)data, length);
}

static int l4Send(l4CPtr_t cptr, l4Word_t offset, void *data, l4Word_t length)
{
	return l4Invoke(cptr, L4_Send, offset, data, length);
}

static int l4Recv(l4CPtr_t cptr, l4Word_t offset, const void *data, l4Word_t length)
{
	return l4Invoke(cptr, L4_Recv, offset, (void*)data, length);
}
