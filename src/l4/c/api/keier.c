#include <l4/api/keier.h>
#include <l4/generic/gfopen.h>
#include <l4/generic/sched.h>
#include <l4/object/endpoint.h>
#include <l4/generic/idspace.h>
#include <l4/enum/rtype.h>
#include <l4/enum/errno.h>

IDSPACE(endpoint);

sl4id_t sys_kopen(l4id_t key)
{
	struct endpoint *ep = ID(endpoint, key);
	if (ep == NULL)
		return -ENOENT;

	return gf_open(ep);
}

int sys_kserve(l4id_t key)
{
	struct endpoint *ep = &current->ep;
	if (IDSET(endpoint, ep, key))
		return -EEXIST;
	return 0;
}
