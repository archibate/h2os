#include <libl4/io.h>
#include <libl4/api.h>
#include <l4/services.h>

int l4Read(l4CPtr_t cptr, l4Word_t offset, void *data, l4Word_t length)
{
	return l4Recv(cptr, L4_Read, offset, data, length);
}

int l4Write(l4CPtr_t cptr, l4Word_t offset, const void *data, l4Word_t length)
{
	return l4Send(cptr, L4_Write, offset, data, length);
}
