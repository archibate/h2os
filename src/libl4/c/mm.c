#include <libl4/mm.h>
#include <libl4/api.h>
#include <l4/services.h>
#include <stddef.h>

int l4Split(l4CPtr_t cptr, l4Word_t point)
{
	return l4Send(cptr, L4_Split, point, 0, 0);
}

int l4Allocate(l4CPtr_t cptr, l4Word_t num, l4Word_t retype)
{
	l4Word_t data[] = {num};
	return l4Send(cptr, L4_Allocate, retype, data, array_sizeof(data));
}
