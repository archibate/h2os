#include <l4/api/keier.h>
#include <l4/generic/idget.h>
#include <l4/generic/gfopen.h>
#include <l4/generic/sched.h>
#include <l4/object/endpoint.h>
#include <l4/enum/rtype.h>
#include <l4/enum/errno.h>

sl4id_t sys_kopen(l4id_t key)
{
	//printk("sys_kopen(%d)", key);//

	struct endpoint *ep = id_get_endpoint(key);
	if (ep == NULL)
		return -ENOENT;

	return gf_open(ep);
}

int sys_kserve(l4id_t key)
{
	//printk("sys_kserve(%d)", key);//

	struct endpoint *ep = &current->ep;

	ep->ide.id = key;
	if (!idg_set_entry(&ep->ide, RTYPE_ENDPOINT))
		return -EEXIST;
	return 0;
}

// int sys_kunserve(void);
